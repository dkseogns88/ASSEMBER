// Fill out your copyright notice in the Description page of Project Settings.


#include "GradNetworkComponent.h"
#include "GradGame/GradLogChannels.h"
#include "GradGame/GradGameplayTags.h"
#include "GradGame/Character/GradPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GradGame/Player/GradPlayerState.h"
#include "GradGame/Inventory/GradInventoryItemDefinition.h"
#include "GradGame/Inventory/GradInventoryItemInstance.h"
#include "GradGame/Equipment/GradEquipmentManagerComponent.h"
#include "GradGame/Equipment/GradEquipmentDefinition.h"
#include "GradGame/Equipment/GradEquipmentInstance.h"
#include "GradGame/Inventory/GradInventoryFragment_EquippableItem.h"


const FName UGradNetworkComponent::NAME_ActorFeatureName("Network");

UGradNetworkComponent::UGradNetworkComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), InventoryList(this)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

UGradNetworkComponent::~UGradNetworkComponent()
{
	delete ObjectInfo;
	delete PosInfo;

	ObjectInfo = nullptr;
	PosInfo = nullptr;
}

void UGradNetworkComponent::OnRegister()
{
	Super::OnRegister();

	// 올바른 Actor에 등록되었는지 확인:
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogGrad, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
			return;
		}
	}

	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (OwningPawn)
	{
		if (OwningPawn->IsPlayerControlled())
		{
			RegisterInitStateFeature();
		}
	}

}

void UGradNetworkComponent::BeginPlay()
{
	Super::BeginPlay();

	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (OwningPawn)
	{
		if (!(OwningPawn->IsPlayerControlled()))
		{
			if (Slots.Num() < NumSlots)
			{
				Slots.AddDefaulted(NumSlots - Slots.Num());
			}
		}
	}
	

	BindOnActorInitStateChanged(UGradPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	ensure(TryToChangeInitState(FGradGameplayTags::Get().InitState_Spawned));

	CheckDefaultInitialization();
}

void UGradNetworkComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (OwningPawn)
	{
		if (OwningPawn->IsPlayerControlled())
		{
			UnregisterInitStateFeature();
		}
	}
	Super::EndPlay(EndPlayReason);
}

void UGradNetworkComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FGradGameplayTags& InitTags = FGradGameplayTags::Get();

	if (Params.FeatureName == UGradPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UGradNetworkComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	const FGradGameplayTags& InitTags = FGradGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	AGradPlayerState* GradPS = GetPlayerState<AGradPlayerState>();

	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		if (!GradPS)
		{
			return false;
		}

		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// PawnExtensionComponent가 DataInitialized될 때까지 기다림 (== 모든 Feature Component가 DataAvailable인 상태)
		return GradPS && Manager->HasFeatureReachedInitState(Pawn, UGradPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UGradNetworkComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	const FGradGameplayTags& InitTags = FGradGameplayTags::Get();

	// DataAvailable -> DataInitialized 단계
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AGradPlayerState* GradPS = GetPlayerState<AGradPlayerState>();
		if (!ensure(Pawn && GradPS))
		{
			return;
		}

		CreateVariables();
	}
}

void UGradNetworkComponent::CheckDefaultInitialization()
{
	const FGradGameplayTags& InitTags = FGradGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	ContinueInitStateChain(StateChain);
}

UGradInventoryItemInstance* UGradNetworkComponent::AddItemDefinition(TSubclassOf<UGradInventoryItemDefinition> ItemDef)
{
	UGradInventoryItemInstance* Result = nullptr;
	if (ItemDef)
	{
		Result = InventoryList.AddEntry(ItemDef);
	}
	return Result;
}

UGradEquipmentManagerComponent* UGradNetworkComponent::FindEquipmentManager() const
{
	return UGradEquipmentManagerComponent::FindEquipManagerComponent(GetOwner());
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

	// 현재 활성화된 ActiveSlotIndex를 활용하여 활성화된 InventoryItemInstance를 찾는다
	if (UGradInventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		// Slot Item을 통해 (InventoryItemInstance) InventoryFragment_EquippableItem의 Fragment를 찾는다:
		// - 찾는 것이 실패했다면, 장착할 수 없는 Inventory Item임을 의미한다
		if (const UGradInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UGradInventoryFragment_EquippableItem>())
		{
			// EquippableItem에서 EquipmentDefinition을 찾는다:
			// - EquipmentDefinition이 있어야, 장착할 수 있다
			TSubclassOf<UGradEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef)
			{
				// 아래는 Unequip이랑 비슷하게 EquipmentManager를 통해 장착한다
				if (UGradEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);

					if (EquippedItem)
					{
						// EquippedItem에는 앞서 보았던 Instiagor로 Slot을 대상으로 넣는다
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
		// UnequipItem/EquipItem을 통해, NewIndex를 통해 할당된 Item을 창착 및 업데이트를 진행한다
		UnequipItemInSlot();
		ActiveSlotIndex = NewIndex;
		EquipItemInSlot();
	}
}

void UGradNetworkComponent::CreateVariables()
{
	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();
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
