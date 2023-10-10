// Fill out your copyright notice in the Description page of Project Settings.


#include "SSGPlayerController.h"
#include "Kismet/GameplayStatics.h"

ASSGPlayerController::ASSGPlayerController()
{
	SessionId = FMath::RandRange(0, 10000);

	// 소켓 연결
	socket.InitSocket();
	if (socket.Connect("127.0.0.1", 7777)) {
		IsConnected = true;
		UE_LOG(LogClass, Log, TEXT("SSGServer Connect Success"));
		socket.SetPlayerController(this);
	}
	
	bNewPlayerEntered = false;

	nPlayers = -1;
	PrimaryActorTick.bCanEverTick = true;
}

ASSGPlayerController::~ASSGPlayerController()
{
}

void ASSGPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!IsConnected) return;

	// 새로운 플레이어 입장
	if (bNewPlayerEntered)
		UpdateNewPlayer();

	auto SSGPlayer = Cast<ASSGWorldCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!SSGPlayer)
		return;

	SSGPlayer->SessionId = SessionId;

	auto MyLocation = SSGPlayer->GetActorLocation();

	cCharacter SSGCharacter;
	SSGCharacter.SessionId = SessionId;
	SSGCharacter.X = MyLocation.X;
	SSGCharacter.Y = MyLocation.Y;
	SSGCharacter.Z = MyLocation.Z;


	if (SSGPlayer && ( IsInputKeyDown(EKeys::W) || IsInputKeyDown(EKeys::A) || IsInputKeyDown(EKeys::S)
		|| IsInputKeyDown(EKeys::D))) {
		socket.SendLocation((float)MyLocation.X, (float)MyLocation.Y, (float)MyLocation.Z);

	}

	// 월드 동기화
	// UpdateWorldInfo();
}

void ASSGPlayerController::BeginPlay()
{
	UE_LOG(LogClass, Log, TEXT("BeginPlay Start"));

	//socket.EnrollPlayer(SSGCharacter);
}

void ASSGPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	closesocket(socket.socket_);
}

void ASSGPlayerController::SendPlayerInfo()
{
}

bool ASSGPlayerController::UpdateWorldInfo()
{
	UWorld* const world = GetWorld();
	if (world == nullptr)
		return false;

	if (CharactersInfo == nullptr)
		return false;

	// 플레이어 업데이트
	UpdatePlayerInfo(CharactersInfo->players[SessionId]);

	return false;
}

void ASSGPlayerController::UpdatePlayerInfo(const cCharacter& info)
{
	auto SSGPlayer = Cast<ASSGWorldCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!SSGPlayer)
		return;
	
	UWorld* const world = GetWorld();
	if (!world)
		return;
}

void ASSGPlayerController::UpdateNewPlayer()
{
}
