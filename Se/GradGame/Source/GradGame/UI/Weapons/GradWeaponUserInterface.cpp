// Fill out your copyright notice in the Description page of Project Settings.

#include "GradWeaponUserInterface.h"
#include "GradGame/Equipment/GradEquipmentManagerComponent.h"
#include "GradGame/Weapons/GradWeaponInstance.h"

UGradWeaponUserInterface::UGradWeaponUserInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UGradWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Pawn을 가져오고
	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		// EquipmentManaterComponent를 활용하여, WeaponInstance를 가져오자
		if (UGradEquipmentManagerComponent* EquipmentManager = Pawn->FindComponentByClass<UGradEquipmentManagerComponent>())
		{
			if (UGradWeaponInstance* NewInstance = EquipmentManager->GetFirstInstanceOfType<UGradWeaponInstance>())
			{
				if (NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr)
				{
					// 새로 업데이트해주고, OnWeaponChanged 호출 진행
					UGradWeaponInstance* OldWeapon = CurrentInstance;
					CurrentInstance = NewInstance;
					OnWeaponChanged(OldWeapon, CurrentInstance);
				}
			}
		}
	}
}