// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Network/Protocol.pb.h"
#include "MyProjectPlayer.generated.h"


UCLASS()
class MYPROJECT_API AMyProjectPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AMyProjectPlayer();
	virtual ~AMyProjectPlayer();

protected:
	virtual void BeginPlay() override ;
	virtual void Tick(float DeltaSeconds) override;

public:
	bool IsMyPlayer();

	Protocol::MoveState GetMoveState() { return PlayerInfo->state(); }
	void SetMoveState(Protocol::MoveState State);

public:
	void SetPlayerInfo(const Protocol::PlayerInfo& Info);
	void SetDestInfo(const Protocol::PlayerInfo& Info);
	Protocol::PlayerInfo* GetPlayerInfo() { return PlayerInfo; }

protected:
	class Protocol::PlayerInfo* PlayerInfo; // 현재 위치
	class Protocol::PlayerInfo* DestInfo; // 목적지
};
