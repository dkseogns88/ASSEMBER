// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ModularCharacter.h"
#include "GradCharacter.generated.h"

class UGradPawnExtensionComponent;
class UGradCameraComponent;


UCLASS()
class GRADGAME_API AGradCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGradCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Grad|Character")
	TObjectPtr<UGradPawnExtensionComponent> PawnExtComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grad|Character")
	TObjectPtr<UGradCameraComponent> CameraComponent;

};
