// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "GradPlayerController.generated.h"

class AGradCharacter;

UCLASS()
class GRADGAME_API AGradPlayerController : public AModularPlayerController
{
	GENERATED_BODY()
public:
	AGradPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;

protected:
	const float MOVE_PACKET_SEND_DELAY = 0.04f;
	float MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;


public:
	// Network Cache
	FVector2D DesiredInput;
	FVector DesiredMoveDirection;
	float DesiredYaw;
	float DesiredPitch;
	float DesiredRoll;

	// Dirty Flag Test
	FVector2D LastDesiredInput;

};
