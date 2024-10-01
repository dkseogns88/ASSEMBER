// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "GradGame/Input/GradMappableConfigPair.h"
#include "GradHeroComponent.generated.h"

struct FInputActionValue;
class UGradCameraMode;

/**
 * component that sets up input and camera handling for player controlled pawns (or bots that simulate players)
 * - this depends on a PawnExtensionComponent to coordinate initialization
 *
 * ī�޶�, �Է� �� �÷��̾ �����ϴ� �ý����� �ʱ�ȭ�� ó���ϴ� ������Ʈ
 */

UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class GRADGAME_API UGradHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
	
public:
	UGradHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** FeatureName ���� */
	static const FName NAME_ActorFeatureName;

	/** Extension Event �̸� ���� */
	static const FName NAME_BindInputsNow;

	/**
	 * UPawnComponent interface
	 */
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	/**
	* IGameFrameworkInitStateInterface
	*/
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) final;
	virtual void CheckDefaultInitialization() final;

	/**
	 * member methods
	 */
	TSubclassOf<UGradCameraMode> DetermineCameraMode() const;
	void InitializePlayerInput(UInputComponent* PlayerInputComponent);
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);

	/**
	* member variables
	*/
	UPROPERTY(EditAnywhere)
	TArray<FGradMappableConfigPair> DefaultInputConfigs;
};
