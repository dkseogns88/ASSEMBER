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


	/** Inventory Item의 인스턴스에는 무엇으로 정의되었는지 메타 클래스인 GradInventoryItemDefinition을 들고 있다 */
	UPROPERTY()
	TSubclassOf<UGradInventoryItemDefinition> ItemDef;
};
