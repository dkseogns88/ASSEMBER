// Fill out your copyright notice in the Description page of Project Settings.


#include "A1GameInstance.h"
#include "Sockets.h"
#include "Engine/World.h"
#include "Characters/Monster.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "Characters/PlayerChar.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
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



UA1GameInstance::UA1GameInstance()
{
	ConstructorHelpers::FClassFinder<AOtherPlayerChar> FindOtherPlayerClass(TEXT("/Script/Engine.Blueprint'/Game/MyBP/BP_OtherPlayer.BP_OtherPlayer_C'"));
	
	if (FindOtherPlayerClass.Succeeded())
	{
		OtherPlayerClass = FindOtherPlayerClass.Class;
	}
}

void UA1GameInstance::Init()
{
	Super::Init();


	//MonsterClass = AEnemy1::StaticClass();


	// 캐릭터 클래스 매핑 초기화
	CharacterBlueprintPaths.Add("Rinty", "Blueprint'/Game/BP/BP_Rinty.BP_Rinty_C'");
	

	//스폰안정화를위해 월드 완전히생성후 텀을두어 몬스터소환
	//GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UMyProjectGameInstance::SpawnNPC, 1.0f, false);


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