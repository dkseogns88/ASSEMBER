// Fill out your copyright notice in the Description page of Project Settings.


#include "GradEquipmentManagerComponent.h"
#include "GradEquipmentDefinition.h"
#include "GradEquipmentInstance.h"
#include "GradEquipmentInstance.h"
#include "GradGame/Character/GradCharacter.h"
#include "GradGame/Network/GradNetCharacter.h"
#include "GradGame/AbilitySystem/GradAbilitySystemComponent.h"

UGradEquipmentInstance* FGradEquipmentList::AddEntry(TSubclassOf<UGradEquipmentDefinition> EquipmentDefinition)
{
	UGradEquipmentInstance* Result = nullptr;
	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	// EquipmentDefinition의 멤버 변수들은 EditDefaultsOnly로 정의되어 있어 GetDefault로 들고 와도 우리에게 필요한 것들이 모두 들어있다
	const UGradEquipmentDefinition* EquipmentCDO = GetDefault<UGradEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UGradEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (!InstanceType)
	{
		InstanceType = UGradEquipmentInstance::StaticClass();
	}

	// Entries에 추가해주자
	FGradAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UGradEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
	Result = NewEntry.Instance;

	UGradAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if(ASC)
	{
		for (const TObjectPtr<UGradAbilitySet> AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, &NewEntry.GrantedHandles, Result);
		}
	}

	// ActorsToSpawn을 통해, Actor들을 인스턴스화 해주자
	// - 어디에? EquipmentInstance에!
	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

	return Result;
}

void FGradEquipmentList::RemoveEntry(UGradEquipmentInstance* Instance)
{
	// 단순히 그냥 Entries를 순회하며, Instance를 찾아서
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FGradAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			UGradAbilitySystemComponent* ASC = GetAbilitySystemComponent();
			check(ASC);
			{
				// TakeFromAbilitySystem은 GiveToAbilitySystem 반대 역활로, ActivatableAbilities에서 제거한다
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			// Actor 제거 작업 및 iterator를 통한 안전하게 Array에서 제거 진행
			Instance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
		}
	}
}

UGradAbilitySystemComponent* FGradEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	if (UGradAbilitySystemComponent* ASC = OwningActor->FindComponentByClass<UGradAbilitySystemComponent>())
	{
		return ASC;
	}

	return nullptr;
}

UGradEquipmentManagerComponent::UGradEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
                                                                                                            , EquipmentList(this)
{
}

UGradEquipmentInstance* UGradEquipmentManagerComponent::EquipItem(TSubclassOf<UGradEquipmentDefinition> EquipmentDefinition)
{
	UGradEquipmentInstance* Result = nullptr;
	if (EquipmentDefinition)
	{
		Result = EquipmentList.AddEntry(EquipmentDefinition);
		if (Result)
		{
			Result->OnEquipped();
		}
	}
	return Result;
}

void UGradEquipmentManagerComponent::UnequipItem(UGradEquipmentInstance* ItemInstance)
{
	if (ItemInstance)
	{
		// 해당 함수는 BP의 Event노드를 호출해준다 (자세한건 해당 함수 구현하면서 보자)
		ItemInstance->OnUnequipped();

		// EquipementList에 제거해준다:
		// - 제거하는 과정을 통해 추가되었던 Actor Instance를 제거를 진행한다
		EquipmentList.RemoveEntry(ItemInstance);
	}
}

UGradEquipmentInstance* UGradEquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<UGradEquipmentInstance> InstanceType)
{
	for (FGradAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UGradEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}
	return nullptr;
}

TArray<UGradEquipmentInstance*> UGradEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<UGradEquipmentInstance> InstanceType) const
{
	TArray<UGradEquipmentInstance*> Results;

	// EquipmentList를 순회하며
	for (const FGradAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UGradEquipmentInstance* Instance = Entry.Instance)
		{
			// InstanceType에 맞는 Class이면 Results에 추가하여 반환
			// - 우리의 경우, GradRangedWeaponInstance가 될거임
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}
