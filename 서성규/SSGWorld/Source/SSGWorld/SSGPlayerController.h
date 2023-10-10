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
	int					SessionId;		// ĳ������ ���� ���� ���̵� (������)
	cCharactersInfo*	CharactersInfo;	// �ٸ� ĳ������ ����

	void SendPlayerInfo();								// �÷��̾� ��ġ �۽�
	bool UpdateWorldInfo();								// ���� ����ȭ
	void UpdatePlayerInfo(const cCharacter& info);		// �÷��̾� ����ȭ	

	// �� �÷��̾� ����
	int	nPlayers;
	bool bNewPlayerEntered;
	cCharacter* NewPlayer;
	void UpdateNewPlayer();
};
