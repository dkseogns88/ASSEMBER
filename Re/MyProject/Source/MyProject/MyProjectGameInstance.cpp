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
	//if (Player == MyPlayer)
	//	return;
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


void UMyProjectGameInstance::Init()
{
	Super::Init();

	// 캐릭터 클래스 매핑 초기화
	CharacterClassMap.Add("Rinty", AMyProjectMyPlayer::StaticClass());
	CharacterClassMap.Add("Sida", AMyProjectMyPlayerSida::StaticClass());
}

TSubclassOf<APawn> UMyProjectGameInstance::FindCharacterClassByName(FString CharacterName)
{
	if (CharacterClassMap.Contains(CharacterName))
	{
		return CharacterClassMap[CharacterName];
	}

	return nullptr;
}

void UMyProjectGameInstance::HandleChange(FString CharacterName)
{
	// 여기서 캐릭터 삭제 후 생성
	// 해결해야 할 건 기존의 캐릭터가 가지고 있던 위치같은 정보들을
	// 다시 새로운 캐릭터에 넘겨줘야 한다. 이거를 서버에서? 아니면 클라에서?
	// 현재 활성화된 플레이어 컨트롤러를 얻음

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController) return;

	// 캐릭터 클래스 찾기 로직을 여기에 추가
	TSubclassOf<APawn> NewCharacterClass = FindCharacterClassByName(CharacterName); 
	if (NewCharacterClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find character class for: %s"), *CharacterName);
		return;
	}

	// 현재 캐릭터 파괴 및 새 캐릭터 스폰
	APawn* CurrentPawn = PlayerController->GetPawn();
	if (CurrentPawn)
	{
		CurrentPawn->Destroy();
	}

	FVector NewSpawnLocation = FVector(0, 0, 100); 
	FRotator NewSpawnRotation = FRotator(0, 0, 0); 
	APawn* NewCharacter = GetWorld()->SpawnActor<APawn>(NewCharacterClass, NewSpawnLocation, NewSpawnRotation);
	if (!NewCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn new character: %s"), *CharacterName);
		return;
	}

	PlayerController->Possess(NewCharacter);
	UE_LOG(LogTemp, Log, TEXT("Character successfully changed to: %s"), *CharacterName);
	
}
