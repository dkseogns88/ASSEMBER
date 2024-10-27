// Fill out your copyright notice in the Description page of Project Settings.


#include "GradInventoryManagerComponent.h"
#include "GradInventoryItemDefinition.h"
#include "GradInventoryItemInstance.h"

UGradInventoryItemInstance* FGradInventoryList::AddEntry(TSubclassOf<UGradInventoryItemDefinition> ItemDef)
{
	UGradInventoryItemInstance* Result = nullptr;
	check(ItemDef);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner(); // 오너는 컨트롤러
	check(OwningActor->HasAuthority());

	FGradInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UGradInventoryItemInstance>(OwningActor);
	NewEntry.Instance->ItemDef = ItemDef;

	// iterating fragments and call callback to OnInstanceCreated()
	for (const UGradInventoryItemFragment* Fragment : GetDefault<UGradInventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}

	Result = NewEntry.Instance;
	return Result;
}

UGradInventoryManagerComponent::UGradInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{}

UGradInventoryItemInstance* UGradInventoryManagerComponent::AddItemDefinition(TSubclassOf<UGradInventoryItemDefinition> ItemDef)
{
	UGradInventoryItemInstance* Result = nullptr;
	if (ItemDef)
	{
		Result = InventoryList.AddEntry(ItemDef);
	}
	return Result;
}
