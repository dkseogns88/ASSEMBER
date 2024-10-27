// Fill out your copyright notice in the Description page of Project Settings.

#include "GradInventoryFragment_SetStats.h"
#include "GradInventoryItemInstance.h"

void UGradInventoryFragment_SetStats::OnInstanceCreated(UGradInventoryItemInstance* Instance) const
{
	// iterating InitialItemStats and add stat tag to InventoryItemInstance
	for (const auto& InitialItemStat : InitialItemStats)
	{
		Instance->AddStatTagStack(InitialItemStat.Key, InitialItemStat.Value);
	}
}