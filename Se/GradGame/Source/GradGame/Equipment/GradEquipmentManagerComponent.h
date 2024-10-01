// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "GradEquipmentManagerComponent.generated.h"

/** forward declarations */
class UGradEquipmentDefinition;
class UGradEquipmentInstance;

USTRUCT(BlueprintType)
struct FGradAppliedEquipmentEntry
{
	GENERATED_BODY()

	/** �������� ���� ��Ÿ ������ */
	UPROPERTY()
	TSubclassOf<UGradEquipmentDefinition> EquipmentDefinition;

	/** EquipmentDefinition�� ���� ������ �ν��Ͻ� */
	UPROPERTY()
	TObjectPtr<UGradEquipmentInstance> Instance = nullptr;
};

/**
 * ����� EquipmentInstance�� �ν��Ͻ��� Entry���� �����ϰ� �ִ�:
 * - GradEquipmentList�� ������ ��ü�� �����Ѵٰ� ���� �ȴ�
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

	/** �������� ���� ���� ����Ʈ */
	UPROPERTY()
	TArray<FGradAppliedEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

/**
 * Pawn�� Component�μ� �������� ���� ������ ����Ѵ�
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

	UPROPERTY()
	FGradEquipmentList EquipmentList;
};
