// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonUserWidget.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "GradWeaponUserInterface.generated.h"

/** forward declaration */
class UGradWeaponInstance;

/**
 * 
 */
UCLASS()
class GRADGAME_API UGradWeaponUserInterface : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UGradWeaponUserInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Weapon 변겅에 따른 BP Event */
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponChanged(UGradWeaponInstance* OldWeapon, UGradWeaponInstance* NewWeapon);

	/**
	 * UUserWidget's interface
	 */
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** 현재 장착된 WeaponInstance를 추적한다 (NativeTick을 활용하여 주기적 업데이트한다) */
	UPROPERTY(Transient)
	TObjectPtr<UGradWeaponInstance> CurrentInstance;
};
