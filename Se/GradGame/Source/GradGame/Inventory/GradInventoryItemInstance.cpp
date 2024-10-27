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
		// GradInventoryItemDefinition은 모든 멤버 변수가 EditDefaultsOnly로 선언되어 있으므로, GetDefault로 가져와도 무관하다
		// - Fragment 정보는 Instance가 아닌 Definition에 있다
		return GetDefault<UGradInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}

void UGradInventoryItemInstance::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void UGradInventoryItemInstance::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 UGradInventoryItemInstance::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool UGradInventoryItemInstance::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}
