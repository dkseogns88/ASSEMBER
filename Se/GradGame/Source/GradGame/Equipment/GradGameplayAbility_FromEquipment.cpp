// Fill out your copyright notice in the Description page of Project Settings.

#include "GradGameplayAbility_FromEquipment.h"
#include "GradEquipmentInstance.h"
#include "GradGame/Inventory/GradInventoryItemInstance.h"



#include "GradGame/Character/GradCharacter.h"
#include "GradGame/Network/GradNetworkComponent.h"
#include "GradGame/Network/NetworkManager.h"
#include "GradGame/Network/GradNetCharacter.h"

UGradEquipmentInstance* UGradGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	// CurrentActorInfo의 AbilitySystemComponent와 CurrentSpecHandle을 활용하여, GameplayAbilitySpec을 가져옴:
	// - CurrentSpecHandle은 SetCurrentActorInfo() 호출할 때, Handle 값을 받아서 저장됨:
	// - CurrentSpecHandle와 CurrentActorInfo은 같이 함
	// - FindAbilitySpecFromHandle은 GiveAbility로 허용된 ActivatableAbilities를 순회하여 GameplayAbilitySpec을 찾아냄
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		// GameplayAbility_FromEquipment는 EquipmentInstance로부터 GiveAbility를 진행했으므로, SourceObject에 EquipmentInstance가 저장되어 있음
		return Cast<UGradEquipmentInstance>(Spec->SourceObject.Get());
	}
	return nullptr;
}

UGradInventoryItemInstance* UGradGameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (UGradEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		// In Lyra, equipment is equipped by inventory item instance:
		// - so, equipment's instigator should be inventory item instance
		// - otherwise, it will return nullptr by failing casting to GradInventoryItemInstance
		return Cast<UGradInventoryItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
}

void UGradGameplayAbility_FromEquipment::TestReload()
{
	AGradCharacter* const AvatarPawn = Cast<AGradCharacter>(GetAvatarActorFromActorInfo());
	if (AvatarPawn)
	{
		if (UGradNetworkComponent* NCP = AvatarPawn->FindComponentByClass<UGradNetworkComponent>())
		{
			Protocol::PosInfo* PosInfo = NCP->GetPosInfo();

			Protocol::C_RELOAD ReloadPkt;
			ReloadPkt.set_object_id(PosInfo->object_id());

			for (auto World : GEngine->GetWorldContexts())
			{
				if (const UGameInstance* GameInstance = World.World()->GetGameInstance())
				{
					if (UNetworkManager* NetworkManager = GameInstance->GetSubsystem<UNetworkManager>())
					{
						NetworkManager->SendPacket(ReloadPkt);
						return;
					}
				}
			}
		}
	}
}
