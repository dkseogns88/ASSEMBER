// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyProject.h"
#include "MyProjectGameInstance.generated.h"

class AMyProjectPlayer;

UCLASS()
class MYPROJECT_API UMyProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
/////////////////////////////////
/////////  Server ///////////////
/////////////////////////////////
public:
	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBuffer);

public:
	void HandleSpawn(const Protocol::ObjectInfo& objectInfo, bool IsMine);
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);

	void HandleDespawn(uint64 ObjectId);
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);

	void HandleMove(const Protocol::S_MOVE& MovePkt);

public:
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;
	TSharedPtr<class PacketSession> GameServerSession;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMyProjectPlayer> OtherPlayerClass;		// 다른 캐릭터

	AMyProjectPlayer* MyPlayer;							// 내 캐릭터
	TMap<uint64, AMyProjectPlayer*> Players;			// 캐릭터들 담아놓은거

};
