// Fill out your copyright notice in the Description page of Project Settings.

#include "GradGameplayAbility_FromEquipment.h"
#include "GradEquipmentInstance.h"
#include "GradGame/Inventory/GradInventoryItemInstance.h"

UGradEquipmentInstance* UGradGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	// CurrentActorInfo�� AbilitySystemComponent�� CurrentSpecHandle�� Ȱ���Ͽ�, GameplayAbilitySpec�� ������:
	// - CurrentSpecHandle�� SetCurrentActorInfo() ȣ���� ��, Handle ���� �޾Ƽ� �����:
	// - CurrentSpecHandle�� CurrentActorInfo�� ���� ��
	// - FindAbilitySpecFromHandle�� GiveAbility�� ���� ActivatableAbilities�� ��ȸ�Ͽ� GameplayAbilitySpec�� ã�Ƴ�
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		// GameplayAbility_FromEquipment�� EquipmentInstance�κ��� GiveAbility�� ���������Ƿ�, SourceObject�� EquipmentInstance�� ����Ǿ� ����
		return Cast<UGradEquipmentInstance>(Spec->SourceObject.Get());
	}
	return nullptr;
}

//UGradInventoryItemInstance* UGradGameplayAbility_FromEquipment::GetAssociatedItem() const
//{
//	if (UGradEquipmentInstance* Equipment = GetAssociatedEquipment())
//	{
//		// In Lyra, equipment is equipped by inventory item instance:
//		// - so, equipment's instigator should be inventory item instance
//		// - otherwise, it will return nullptr by failing casting to GradInventoryItemInstance
//		return Cast<UGradInventoryItemInstance>(Equipment->GetInstigator());
//	}
//	return nullptr;
//}