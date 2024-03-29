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
		
	void HandleJump(const Protocol::S_JUMP& JumpPkt);

	void HandleChange(FString CharacterName);
	// 캐릭터 클래스 찾기 함수
	TSubclassOf<APawn> FindCharacterClassByName(FString CharacterName);

	// 초기화 함수
	virtual void Init() override;


private:
	// 캐릭터 이름과 클래스를 매핑하는 맵
	TMap<FString, TSubclassOf<APawn>> CharacterClassMap;

public:
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;
	TSharedPtr<class PacketSession> GameServerSession;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMyProjectPlayer> OtherPlayerClass;		// �ٸ� ĳ����

	AMyProjectPlayer* MyPlayer;							// �� ĳ����
	TMap<uint64, AMyProjectPlayer*> Players;			// ĳ���͵� ��Ƴ�����

};
