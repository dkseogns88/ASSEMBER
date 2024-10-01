// Fill out your copyright notice in the Description page of Project Settings.


#include "GradInventoryItemInstance.h"

#include "GradInventoryItemDefinition.h"

UGradInventoryItemInstance::UGradInventoryItemInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const UGradInventoryItemFragment* UGradInventoryItemInstance::FindFragmentByClass(TSubclassOf<UGradInventoryItemFragment> FragmentClass) const
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		// GradInventoryItemDefinition�� ��� ��� ������ EditDefaultsOnly�� ����Ǿ� �����Ƿ�, GetDefault�� �����͵� �����ϴ�
		// - Fragment ������ Instance�� �ƴ� Definition�� �ִ�
		return GetDefault<UGradInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}
