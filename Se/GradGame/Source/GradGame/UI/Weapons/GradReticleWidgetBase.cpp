#include "GradReticleWidgetBase.h"
#include "GradGame/Weapons/GradWeaponInstance.h"
#include "GradGame/Inventory/GradInventoryItemInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GradReticleWidgetBase)

UGradReticleWidgetBase::UGradReticleWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void UGradReticleWidgetBase::InitializeFromWeapon(UGradWeaponInstance* InWeapon)
{
	WeaponInstance = InWeapon;
	InventoryInstance = nullptr;
	if (WeaponInstance)
	{
		InventoryInstance = Cast<UGradInventoryItemInstance>(WeaponInstance->GetInstigator());
	}

	OnWeaponInitialized();
}