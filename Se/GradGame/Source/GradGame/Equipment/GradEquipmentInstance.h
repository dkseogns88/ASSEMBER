// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GradEquipmentInstance.generated.h"

struct FGradEquipmentActorToSpawn;
/**
 * 헬멧, 신발 등 다른 장착할 수 있는 것들은 UGradEquipmentInstance을 상속받아서 만들면 된다.
 * UGradEquipmentInstance는 관리자로 생각하면 좋다. 아이템이 소환되면 관리 방식을 결정하는게 UGradEquipmentInstance다. 
 */
UCLASS(BlueprintType, Blueprintable)
class GRADGAME_API UGradEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	UGradEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* Blueprint 정의를 위한 Equip/Unequip 함수
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnUnequipped"))
	void K2_OnUnequipped();

	UFUNCTION(BlueprintPure, Category = Equipment)
	TArray<AActor*> GetSpawnedActors() const { return SpawnedActors; }

	UFUNCTION(BlueprintPure, Category = Equipment)
	UObject* GetInstigator() const { return Instigator; }

	UFUNCTION(BlueprintPure, Category = Equipment)
	APawn* GetPawn() const;

	void SpawnEquipmentActors(const TArray<FGradEquipmentActorToSpawn>& ActorsToSpawn);
	void DestroyEquipmentActors();

	/**
	 * DeterminesOutputType은 C++ 정의에는 APawn* 반환하지만, BP에서는 PawnType에 따라 OutputType이 결정되도록 리다이렉트(Redirect)한다
	 */
	UFUNCTION(BlueprintPure, Category = Equipment, meta = (DeterminesOutputType = PawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;

	/**
	* interfaces
	*/
	virtual void OnEquipped();
	virtual void OnUnequipped();

	/** 어떤 InventoryItemInstance에 의해 활성화되었는지 (추후, QuickBarComponent에서 보게 될것이다) */
	UPROPERTY()
	TObjectPtr<UObject> Instigator;

	/** GradEquipementDefinition에 맞게 Spawn된 Actor Instance들 */
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
