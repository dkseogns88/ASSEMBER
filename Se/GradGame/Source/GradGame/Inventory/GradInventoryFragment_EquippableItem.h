#pragma once

#include "GradInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"
#include "GradInventoryFragment_EquippableItem.generated.h"

/** forward declaration */
class UGradEquipmentDefinition;

UCLASS()
class UGradInventoryFragment_EquippableItem : public UGradInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Grad)
	TSubclassOf<UGradEquipmentDefinition> EquipmentDefinition;
};