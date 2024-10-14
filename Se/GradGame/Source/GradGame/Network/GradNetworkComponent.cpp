// Fill out your copyright notice in the Description page of Project Settings.


#include "GradNetworkComponent.h"
#include "GradGame/GradLogChannels.h"
#include "GradGame/GradGameplayTags.h"
#include "GradGame/Character/GradPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GradGame/Player/GradPlayerState.h"
#include "GradGame/Equipment/GradEquipmentManagerComponent.h"
#include "GradGame/Equipment/GradEquipmentInstance.h"
#include "GradGame/Equipment/GradEquipmentDefinition.h"
#include "GradGame/Inventory/GradInventoryFragment_EquippableItem.h"
#include "GradGame/Inventory/GradInventoryItemInstance.h"

UGradNetworkComponent::UGradNetworkComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();

	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}
}

UGradNetworkComponent::~UGradNetworkComponent()
{
	delete ObjectInfo;
	delete PosInfo;

	ObjectInfo = nullptr;
	PosInfo = nullptr;
}

UGradEquipmentManagerComponent* UGradNetworkComponent::FindEquipmentManager() const
{	
	if (APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		return Pawn->FindComponentByClass<UGradEquipmentManagerComponent>();
	}

	return nullptr;
}

void UGradNetworkComponent::UnequipItemInSlot()
{
	if (UGradEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		if (EquippedItem)
		{
			EquipmentManager->UnequipItem(EquippedItem);

			EquippedItem = nullptr;
		}
	}
}

void UGradNetworkComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);

	if (UGradInventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		if (const UGradInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UGradInventoryFragment_EquippableItem>())
		{
			TSubclassOf<UGradEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef)
			{
				if (UGradEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);

					if (EquippedItem)
					{
						EquippedItem->Instigator = SlotItem;
					}
				}
			}
		}
	}
}

void UGradNetworkComponent::AddItemToSlot(int32 SlotIndex, UGradInventoryItemInstance* Item)
{
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
		}
	}
}

void UGradNetworkComponent::SetActiveSlotIndex(int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		UnequipItemInSlot();
		ActiveSlotIndex = NewIndex;
		EquipItemInSlot();
	}
}

void UGradNetworkComponent::SetMoveState(Protocol::MoveState State)
{
	if (PosInfo->move_state() == State)
		return;

	PosInfo->set_move_state(State);
}

void UGradNetworkComponent::SetObjectInfo(const Protocol::ObjectInfo& Info)
{
	if (ObjectInfo->object_id() != 0)
	{
		assert(ObjectInfo->object_id() == Info.object_id());
	}

	ObjectInfo->CopyFrom(Info);
}

void UGradNetworkComponent::SetPosInfo(const Protocol::PosInfo& Info)
{
	if (PosInfo->object_id() != 0)
	{
		assert(PosInfo->object_id() == Info.object_id());
	}

	PosInfo->CopyFrom(Info);
	SetMoveState(Info.move_state());
}
