// Fill out your copyright notice in the Description page of Project Settings.


#include "A1GameInstance.h"
#include "Sockets.h"
#include "Engine/World.h"
#include "Characters/Monster.h"
#include "TimerManager.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "Characters/PlayerChar.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Pawn.h"
#include "Characters/BaseChar.h"
#include "Characters/OtherPlayerChar.h"
#include "Objects/A1PlayerController.h"
#include "Objects/AnimInstanceCustom.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/DamageType.h"
#include "UObject/UObjectGlobals.h"
#include "Engine/StaticMeshActor.h"
#include "NavigationSystem.h"
#include "NavMesh/RecastNavMesh.h"
#include "NavMesh/RecastHelpers.h"
#include "NavMesh/RecastNavMeshDataChunk.h"
#include "DrawDebugHelpers.h"
#include "NavFilters/NavigationQueryFilter.h"
#include "EngineUtils.h"


UA1GameInstance::UA1GameInstance()
{
	NextSpawnLocation = FVector(1350.0f, 1900.0f, 100.0f);

	ConstructorHelpers::FClassFinder<AOtherPlayerChar> FindOtherPlayerClass(TEXT("/Script/Engine.Blueprint'/Game/MyBP/BP_OtherPlayer.BP_OtherPlayer_C'"));
	
	if (FindOtherPlayerClass.Succeeded())
	{
		OtherPlayerClass = FindOtherPlayerClass.Class;
	}

	ConstructorHelpers::FClassFinder<AMonster> FindMonkClass(TEXT("/Script/Engine.Blueprint'/Game/MyBP/BP_Monk.BP_Monk_C'"));

	if (FindMonkClass.Succeeded())
	{
		BPClassMonk = FindMonkClass.Class;
	}

	ConstructorHelpers::FClassFinder<AMonster> FindFanaticClass(TEXT("/Script/Engine.Blueprint'/Game/MyBP/BP_Fanatic.BP_Fanatic_C'"));

	if (FindFanaticClass.Succeeded())
	{
		BPClassFanatic = FindFanaticClass.Class;
	}

}

void UA1GameInstance::Init()
{
	Super::Init();

	// 캐릭터 클래스 매핑 초기화
	CharacterBlueprintPaths.Add("Rinty", "Blueprint'/Game/BP/BP_Rinty.BP_Rinty_C'");
	
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UA1GameInstance::SpawnMonsters, 1.0f, false);

	LogNavMeshPolygons();
	
}



void UA1GameInstance::SpawnMonsters()
{
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, [this]()
		{
			
			SpawnMonster(BPClassMonk);
			SpawnMonster(BPClassFanatic);
		}, 3.0f, true);
}

//몬스터스폰 충돌검사
bool UA1GameInstance::SpawnLocationValid(FVector Location, FRotator Rotation, FVector BoxExtent)
{
	FHitResult HitResult;
	return !UKismetSystemLibrary::BoxTraceSingle(
		this,
		Location,
		Location,
		BoxExtent,
		Rotation,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		HitResult,
		true
	);
}

void UA1GameInstance::SpawnMonster(TSubclassOf<AMonster> MonsterClass)
{
	if (!MonsterClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	FVector Location = NextSpawnLocation;
	FRotator Rotation = FRotator::ZeroRotator; 
	FVector BoxExtent(50.0f, 50.0f, 50.0f); 

	// 충돌 검사
	while (!SpawnLocationValid(Location, Rotation, BoxExtent))
	{
		Location.X += 10.0f;
		Location.Y += 10.0f;
	}

	FActorSpawnParameters SpawnParams;
	AMonster* SpawnedMonster = World->SpawnActor<AMonster>(MonsterClass, Location, Rotation, SpawnParams);
	if (SpawnedMonster)
	{

		uint64 MonsterID = FGuid::NewGuid().A; // 고유 ID 생성
		Monsters.Add(MonsterID, SpawnedMonster);
		UE_LOG(LogTemp, Log, TEXT("Spawned Monster: %s at Location: %s"), *MonsterClass->GetName(), *Location.ToString());

		NextSpawnLocation.X += 10.0f;
		NextSpawnLocation.Y += 10.0f;

		LogNavMeshPolygons();
	}
	
	
}

UClass* UA1GameInstance::GetCharacterClass(const FString& CharacterName) const
{
	const FString* FoundPath = CharacterBlueprintPaths.Find(CharacterName);
	if (FoundPath)
	{
		FString AssetPath = *FoundPath + _TEXT("_C"); // Add "_C" to the path to get the UClass path
		UE_LOG(LogTemp, Log, TEXT("Attempting to load class at path: %s"), *AssetPath);
		UClass* BlueprintClass = StaticLoadClass(UObject::StaticClass(), nullptr, *AssetPath);
		if (BlueprintClass)
		{
			UE_LOG(LogTemp, Log, TEXT("Successfully loaded class for: %s"), *CharacterName);
			return BlueprintClass;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load class at path: %s"), *AssetPath);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No path found for character name: %s"), *CharacterName);
	}
	return nullptr;
}

void UA1GameInstance::LogCharacterChange(int32 PlayerIndex, const FString& NewCharacterName)
{
	FPlayerCharacterChangeInfo ChangeInfo(PlayerIndex, NewCharacterName);
	PlayerCharacterChangeLog.Add(PlayerIndex, NewCharacterName);


	//캐릭터 변경에대한 정보를 로그로 출력
	UE_LOG(LogTemp, Log, TEXT("Player %d changed to character: %s"), PlayerIndex, *NewCharacterName);
	for (const TPair<int32, FString>& Info : PlayerCharacterChangeLog)
	{
		UE_LOG(LogTemp, Log, TEXT("Log - Player %d: %s"), Info.Key, *Info.Value);
	}
}

FVector Recast2UnrVector(const float* RecastCoords)
{
	return FVector(RecastCoords[0], RecastCoords[1], RecastCoords[2]);
}

void UA1GameInstance::LogNavMeshPolygons()
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys == nullptr) return;

	ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(NavSys->GetMainNavData());
	if (NavMesh == nullptr) return;

	const FBox NavBounds = NavMesh->GetNavMeshBounds();

	
	const float TileSize = NavMesh->TileSizeUU;
	const FIntPoint TileCount = FIntPoint(
		FMath::CeilToInt(NavBounds.GetSize().X / TileSize),
		FMath::CeilToInt(NavBounds.GetSize().Y / TileSize)
	);

	
	TArray<FVector> TileCenters;

	
	for (int32 TileY = 0; TileY < TileCount.Y; ++TileY)
	{
		for (int32 TileX = 0; TileX < TileCount.X; ++TileX)
		{
			FVector TileCenter = FVector(
				NavBounds.Min.X + TileX * TileSize + TileSize / 2,
				NavBounds.Min.Y + TileY * TileSize + TileSize / 2,
				NavBounds.GetCenter().Z
			);

			TileCenters.Add(TileCenter);
		}
	}

	
	for (int32 TileX = 0; TileX < TileCount.X - 1; ++TileX)
	{
		for (int32 TileY = 0; TileY < TileCount.Y - 1; ++TileY)
		{
			//상대좌표
			FVector Triangle1[] = {
				FVector(TileX, TileY, 0),
				FVector(TileX + 1, TileY, 0),
				FVector(TileX, TileY + 1, 0)
			};

			FVector Triangle2[] = {
				FVector(TileX + 1, TileY, 0),
				FVector(TileX, TileY + 1, 0),
				FVector(TileX + 1, TileY + 1, 0)
			};

			//절대좌표
			FVector WorldTriangle1[] = {
				TileCenters[TileX + TileY * TileCount.X],
				TileCenters[(TileX + 1) + TileY * TileCount.X],
				TileCenters[TileX + (TileY + 1) * TileCount.X]
			};

			FVector WorldTriangle2[] = {
				TileCenters[(TileX + 1) + TileY * TileCount.X],
				TileCenters[TileX + (TileY + 1) * TileCount.X],
				TileCenters[(TileX + 1) + (TileY + 1) * TileCount.X]
			};

			
			UE_LOG(LogTemp, Log, TEXT("Triangle %s -> %s, %s, %s"), *FString::Printf(TEXT("{(%.0f,%.0f),(%.0f,%.0f),(%.0f,%.0f)}"), Triangle1[0].X, Triangle1[0].Y, Triangle1[1].X, Triangle1[1].Y, Triangle1[2].X, Triangle1[2].Y), *WorldTriangle1[0].ToString(), *WorldTriangle1[1].ToString(), *WorldTriangle1[2].ToString());
			UE_LOG(LogTemp, Log, TEXT("Triangle %s -> %s, %s, %s"), *FString::Printf(TEXT("{(%.0f,%.0f),(%.0f,%.0f),(%.0f,%.0f)}"), Triangle2[0].X, Triangle2[0].Y, Triangle2[1].X, Triangle2[1].Y, Triangle2[2].X, Triangle2[2].Y), *WorldTriangle2[0].ToString(), *WorldTriangle2[1].ToString(), *WorldTriangle2[2].ToString());
		}
	}
}