// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "GradPawnExtensionComponent.generated.h"

class UGradPawnData;
class UGradAbilitySystemComponent;

/**
 * 초기화 전반을 조정하는 컴포넌트
 * - 전체적인 모든 컴포넌트 초기화에 관련된 중재를 하준다.
 */
UCLASS()
class GRADGAME_API UGradPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
	
	public:
	UGradPawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** FeatureName 정의 */
	static const FName NAME_ActorFeatureName;

	/**
	* member methods
	*/
	static UGradPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UGradPawnExtensionComponent>() : nullptr); }
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const UGradPawnData* InPawnData);
	void SetupPlayerInputComponent();
	UGradAbilitySystemComponent* GetGradAbilitySystemComponent() const { return AbilitySystemComponent; }

	/** AbilitySystemComponent의 AvatorActor 대상 초기화/해제 호출 */
	void InitializeAbilitySystem(UGradAbilitySystemComponent* InASC, AActor* InOwnerActor);
	void UninitializeAbilitySystem();
	
	/** OnAbilitySystem[Initialized|Uninitialized] Delegate에 추가: */
	void OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);
	void OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate);

	/**
	* UPawnComponent interfaces
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
	virtual void CheckDefaultInitialization() final;

	/**
	 * Pawn을 생성한 데이터를 캐싱
	 */
	UPROPERTY(EditInstanceOnly, Category = "Grad|Pawn")
	TObjectPtr<const UGradPawnData> PawnData;

	/** AbilitySystemComponent 캐싱 */
	UPROPERTY()
	TObjectPtr<UGradAbilitySystemComponent> AbilitySystemComponent;

	/** ASC Init과 Uninit의 Delegate 추가 */
	FSimpleMulticastDelegate OnAbilitySystemInitialized;
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;
};
