// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "cSocket.h"
#include "SSGWorldCharacter.h"
#include "SSGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SSGWORLD_API ASSGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASSGPlayerController();
	~ASSGPlayerController();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


private:
	cSocket				socket;
	bool				IsConnected{ false };
	int					SessionId;		// 캐릭터의 세션 고유 아이디 (랜덤값)
	cCharactersInfo*	CharactersInfo;	// 다른 캐릭터의 정보

	void SendPlayerInfo();								// 플레이어 위치 송신
	bool UpdateWorldInfo();								// 월드 동기화
	void UpdatePlayerInfo(const cCharacter& info);		// 플레이어 동기화	

	// 새 플레이어 입장
	int	nPlayers;
	bool bNewPlayerEntered;
	cCharacter* NewPlayer;
	void UpdateNewPlayer();
};
