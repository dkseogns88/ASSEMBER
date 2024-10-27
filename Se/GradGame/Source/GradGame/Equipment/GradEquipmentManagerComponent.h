// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "GradGame/AbilitySystem/GradAbilitySet.h"
#include "GradEquipmentManagerComponent.generated.h"

/** forward declarations */
class UGradEquipmentDefinition;
class UGradEquipmentInstance;

USTRUCT(BlueprintType)
struct FGradAppliedEquipmentEntry
{
	GENERATED_BODY()

	/** 장착물에 대한 메타 데이터 */
	UPROPERTY()
	TSubclassOf<UGradEquipmentDefinition> EquipmentDefinition;

	/** EquipmentDefinition을 통해 생성된 인스턴스 */
	UPROPERTY()
	TObjectPtr<UGradEquipmentInstance> Instance = nullptr;

	/** 무기에 할당된 허용가능한 GameplayAbility */
	UPROPERTY()
	FGradAbilitySet_GrantedHandles GrantedHandles;
};

/**
 * 참고로 EquipmentInstance의 인스턴스를 Entry에서 관리하고 있다:
 * - GradEquipmentList는 생성된 객체를 관리한다고 보면 된다
 */
USTRUCT(BlueprintType)
struct FGradEquipmentList
{
	GENERATED_BODY()

	FGradEquipmentList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{}

	UGradEquipmentInstance* AddEntry(TSubclassOf<UGradEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UGradEquipmentInstance* Instance);

	UGradAbilitySystemComponent* GetAbilitySystemComponent() const;

	/** 장착물에 대한 관리 리스트 */
	UPROPERTY()
	TArray<FGradAppliedEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

/**
 * Pawn의 Component로서 장착물에 대한 관리를 담당한다
 */
UCLASS()
class GRADGAME_API UGradEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
public:
	UGradEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* Network methods
	*/
	static UGradEquipmentManagerComponent* FindEquipManagerComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UGradEquipmentManagerComponent>() : nullptr); }

	UGradEquipmentInstance* EquipItem(TSubclassOf<UGradEquipmentDefinition> EquipmentDefinition);
	void UnequipItem(UGradEquipmentInstance* ItemInstance);

	/** 장착물 중 처음 것을 반환 없으면 NULL */
	UGradEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UGradEquipmentInstance> InstanceType);

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

	UFUNCTION(BlueprintCallable)
	TArray<UGradEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UGradEquipmentInstance> InstanceType) const;

	UPROPERTY()
	FGradEquipmentList EquipmentList;
};
