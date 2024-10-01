// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GradInventoryItemInstance.generated.h"

class UGradInventoryItemFragment;
class UGradInventoryItemDefinition;

/**
 * 
 */
UCLASS()
class GRADGAME_API UGradInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	UGradInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	const UGradInventoryItemFragment* FindFragmentByClass(TSubclassOf<UGradInventoryItemFragment> FragmentClass) const;

	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}


	/** Inventory Item�� �ν��Ͻ����� �������� ���ǵǾ����� ��Ÿ Ŭ������ GradInventoryItemDefinition�� ��� �ִ� */
	UPROPERTY()
	TSubclassOf<UGradInventoryItemDefinition> ItemDef;
};
