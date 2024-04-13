// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProjectGameInstance.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "ServerPacketHandler.h"
#include "MyProjectPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "MyProjectMyPlayer.h"
#include "MyProjectMyPlayerSida.h"
#include "MyProjectPlayerController.h"

void UMyProjectGameInstance::ConnectToGameServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));
	
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();

		// �α��� �õ�(���Ŀ� ���̵�, ��� �߰�?)
		Protocol::C_LOGIN Pkt;
		SendBufferRef SendBuffer = ServerPacketHandler::MakeSendBuffer(Pkt);
		SendPacket(SendBuffer);

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

void UMyProjectGameInstance::DisconnectToGameServer()
{

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	Protocol::C_LEAVE_GAME LeavePkt;
	SEND_PACKET(LeavePkt);

	/*if (Socket)
	{
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
	}*/
}

void UMyProjectGameInstance::HandleRecvPackets()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->HandleRecvPackets();
}

void UMyProjectGameInstance::SendPacket(SendBufferRef SendBuffer)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->SendPacket(SendBuffer);
}

void UMyProjectGameInstance::HandleSpawn(const Protocol::ObjectInfo& objectInfo, bool IsMine)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	// �ߺ� ó�� üũ
	const uint64 ObjectId = objectInfo.object_id();
	if (Players.Find(ObjectId) != nullptr)
		return;

	FVector SpawnLocation(objectInfo.pos_info().x(), objectInfo.pos_info().y(), objectInfo.pos_info().z());

	if (IsMine)
	{
		auto* PC = UGameplayStatics::GetPlayerController(this, 0);
		AMyProjectPlayer* Player = Cast<AMyProjectPlayer>(PC->GetPawn());
		if (Player == nullptr)
			return;

		Player->SetPlayerInfo(objectInfo.pos_info());
		MyPlayer = Player;
		Players.Add(objectInfo.object_id(), Player);
	}
	else
	{
		AMyProjectPlayer* Player = Cast<AMyProjectPlayer>(World->SpawnActor(OtherPlayerClass, &SpawnLocation));
		Player->SetPlayerInfo(objectInfo.pos_info());
		Players.Add(objectInfo.object_id(), Player);
	}

}

void UMyProjectGameInstance::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	HandleSpawn(EnterGamePkt.player(), true);
}

void UMyProjectGameInstance::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	for (auto& Player : SpawnPkt.players())
	{
		HandleSpawn(Player, false);
	}
}

void UMyProjectGameInstance::HandleDespawn(uint64 ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	AMyProjectPlayer** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	World->DestroyActor(*FindActor);
}

void UMyProjectGameInstance::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	for (auto& ObjectId : DespawnPkt.object_ids())
	{
		HandleDespawn(ObjectId);
	}
}

void UMyProjectGameInstance::HandleMove(const Protocol::S_MOVE& MovePkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = MovePkt.info().object_id();
	AMyProjectPlayer** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	AMyProjectPlayer* Player = (*FindActor);

	if (Player->IsMyPlayer())
		return;

	const Protocol::PosInfo& Info = MovePkt.info();
	Player->SetDestInfo(Info);
}

void UMyProjectGameInstance::HandleJump(const Protocol::S_JUMP& JumpPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = JumpPkt.info().object_id();
	AMyProjectPlayer** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	AMyProjectPlayer* Player = (*FindActor);
	if (Player->IsMyPlayer())
		return;

	const Protocol::PosInfo& Info = JumpPkt.info();
	Player->SetDestInfo(Info);
}

void UMyProjectGameInstance::HandleZoom(const Protocol::S_ZOOM& ZommPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = ZommPkt.info().object_id();
	AMyProjectPlayer** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	AMyProjectPlayer* Player = (*FindActor);
	if (Player->IsMyPlayer())
		return;

	// 여기서부터 다른 클라이언트 줌 애니메이션 처리


}


void UMyProjectGameInstance::Init()
{
	Super::Init();

	// 캐릭터 클래스 매핑 초기화
	CharacterBlueprintPaths.Add("Rinty", "Blueprint'/Game/MyBP/BP_Class/BP_MyPlayer.BP_MyPlayer_C'");
	CharacterBlueprintPaths.Add("Sida", "Blueprint'/Game/MyBP/BP_Class/BP_MyPlayer_sida.BP_MyPlayer_sida_C'");
	
}

TSubclassOf<APawn> UMyProjectGameInstance::FindCharacterClassByName(const FString& CharacterName)
{
	// 캐릭터 이름으로 함수 호출 확인 로그 출력
	UE_LOG(LogTemp, Log, TEXT("FindCharacterClassByName called with CharacterName: %s"), *CharacterName);

	if (CharacterBlueprintPaths.Contains(CharacterName))
	{
		FString Path = CharacterBlueprintPaths[CharacterName];
		// 경로를 찾았을 때의 로그 출력
		UE_LOG(LogTemp, Log, TEXT("Path found for %s: %s"), *CharacterName, *Path);

		UClass* Class = LoadClass<APawn>(nullptr, *Path);
		if (Class)
		{
			// 클래스 로드 성공 로그 출력
			UE_LOG(LogTemp, Log, TEXT("Successfully loaded class for %s"), *CharacterName);
			return Class;
		}
		else
		{
			// 클래스 로드 실패 로그 출력
			UE_LOG(LogTemp, Warning, TEXT("Failed to load class for %s"), *CharacterName);
		}
	}
	else
	{
		// 캐릭터 이름에 해당하는 경로를 찾지 못했을 때의 로그 출력
		UE_LOG(LogTemp, Warning, TEXT("No path found for character name: %s"), *CharacterName);
	}

	return nullptr;
}
void UMyProjectGameInstance::HandleChange(const FString& CharacterName)
{

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this,0);
	if (!PlayerController) return;

	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // 충돌과 무관하게 항상 스폰

	TSubclassOf<APawn> NewCharacterClassInInstance = FindCharacterClassByName(CharacterName); 
	if (NewCharacterClassInInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find character class for: %s"), *CharacterName);
		return;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Success to find character class for: %s"), *CharacterName);
		
	}
	FVector NewSpawnLocationAtInstance = FVector(0, 0, 100); 
	FRotator NewSpawnRotationAtInstance = FRotator(0, 0, 0); 

	// 현재 캐릭터 파괴 및 새 캐릭터 스폰
	APawn* CurrentPawn = PlayerController->GetPawn();
	if (CurrentPawn)
	{
		CurrentPawn->Destroy();
	}

	UE_LOG(LogTemp, Log, TEXT("Attempting to spawn character at Location: %s, Rotation: %s"),
		*NewSpawnLocationAtInstance.ToString(), *NewSpawnRotationAtInstance.ToString());

	APawn* NewSpawnCharacter = GetWorld()->SpawnActor<APawn>(NewCharacterClassInInstance, NewSpawnLocationAtInstance, NewSpawnRotationAtInstance,SpawnParams);
	if (!NewSpawnCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn new character: %s"), *CharacterName);
		return;
	}
	else
	{
		
		UE_LOG(LogTemp, Log, TEXT("New character class is: %s"), *NewCharacterClassInInstance->GetName());

		
		UE_LOG(LogTemp, Log, TEXT("New character spawned at Location: %s"), *NewSpawnCharacter->GetActorLocation().ToString());

		// 새 캐릭터로 플레이어 소유권 변경
		PlayerController->Possess(NewSpawnCharacter);
		UE_LOG(LogTemp, Log, TEXT("INSTANCE : Character successfully changed to: %s"), *CharacterName);
		
		
		AMyProjectPlayerController* MyController = Cast<AMyProjectPlayerController>(PlayerController);
		if (MyController)
		{
			// UI 비활성화 및 게임 모드로 전환
			MyController->bIsUIActive = false;
			MyController->ToggleCharacterSelectUI(); 
		}

		
	}
}
