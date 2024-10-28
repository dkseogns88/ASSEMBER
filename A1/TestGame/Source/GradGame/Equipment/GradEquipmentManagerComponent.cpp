// Fill out your copyright notice in the Description page of Project Settings.


#include "GradEquipmentManagerComponent.h"
#include "GradEquipmentDefinition.h"
#include "GradEquipmentInstance.h"
#include "GradEquipmentInstance.h"
#include "GradGame/Character/GradCharacter.h"
#include "GradGame/Network/GradNetCharacter.h"

UGradEquipmentInstance* FGradEquipmentList::AddEntry(TSubclassOf<UGradEquipmentDefinition> EquipmentDefinition)
{
	UGradEquipmentInstance* Result = nullptr;
	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	// EquipmentDefinition�� ��� �������� EditDefaultsOnly�� ���ǵǾ� �־� GetDefault�� ��� �͵� �츮���� �ʿ��� �͵��� ��� ����ִ�
	const UGradEquipmentDefinition* EquipmentCDO = GetDefault<UGradEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UGradEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (!InstanceType)
	{
		InstanceType = UGradEquipmentInstance::StaticClass();
	}

	// Entries�� �߰�������
	FGradAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UGradEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
	Result = NewEntry.Instance;

	// ActorsToSpawn�� ����, Actor���� �ν��Ͻ�ȭ ������
	// - ���? EquipmentInstance��!
	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

	return Result;
}

void FGradEquipmentList::RemoveEntry(UGradEquipmentInstance* Instance)
{
	// �ܼ��� �׳� Entries�� ��ȸ�ϸ�, Instance�� ã�Ƽ�
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FGradAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			// Actor ���� �۾� �� iterator�� ���� �����ϰ� Array���� ���� ����
			Instance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
		}
	}
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
			/*if (Cast<AGradCharacter>(GetOwner()))
			{

			}
			else if(Cast<AGradNetCharacter>(GetOwner()))*/

			Result->OnEquipped();
		}
	}
	return Result;
}

void UGradEquipmentManagerComponent::UnequipItem(UGradEquipmentInstance* ItemInstance)
{
	if (ItemInstance)
	{
		// �ش� �Լ��� BP�� Event��带 ȣ�����ش� (�ڼ��Ѱ� �ش� �Լ� �����ϸ鼭 ����)
		ItemInstance->OnUnequipped();

		// EquipementList�� �������ش�:
		// - �����ϴ� ������ ���� �߰��Ǿ��� Actor Instance�� ���Ÿ� �����Ѵ�
		EquipmentList.RemoveEntry(ItemInstance);
	}
}
