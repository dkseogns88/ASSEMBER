// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "Protocol.pb.h"
#include "../Inventory/GradInventoryManagerComponent.h"
#include "GradNetworkComponent.generated.h"

class UGradEquipmentManagerComponent;
class UGradEquipmentInstance;
class UGradInventoryItemInstance;

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class GRADGAME_API UGradNetworkComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
public:
	UGradNetworkComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	~UGradNetworkComponent();

	/** FeatureName ���� */
	static const FName NAME_ActorFeatureName;

	/**
	 * UPawnComponent interface
	 */
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	/**
	* IGameFrameworkInitStateInterface
	*/
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) final;
	virtual void CheckDefaultInitialization() final;

	/**
	* CopyInventoryManager
	*/
	/** InventoryItemDefinition�� ����, InventoryList�� �߰��Ͽ� �����ϸ�, InventoryItemInstance�� ��ȯ�Ѵ� */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	UGradInventoryItemInstance* AddItemDefinition(TSubclassOf<UGradInventoryItemDefinition> ItemDef);

	UPROPERTY()
	FGradInventoryList InventoryList;

	/**
	* CopyQuickBarComponent
	*/
	UGradEquipmentManagerComponent* FindEquipmentManager() const;
	void UnequipItemInSlot();
	void EquipItemInSlot();

	UFUNCTION(BlueprintCallable)
	void AddItemToSlot(int32 SlotIndex, UGradInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable, Category = "Grad")
	void SetActiveSlotIndex(int32 NewIndex);

	/** HUD QuickBar Slot ���� */
	UPROPERTY()
	int32 NumSlots = 3;

	/** HUD QuickBar Slot ����Ʈ */
	UPROPERTY()
	TArray<TObjectPtr<UGradInventoryItemInstance>> Slots;

	/** ���� Ȱ��ȭ�� Slot Index (�Ƹ� Lyra�� �� �Ѱ��� Slot�� Ȱ��ȭ�Ǵ°�����?) */
	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	/** ���� ������ ��� ���� */
	UPROPERTY()
	TObjectPtr<UGradEquipmentInstance> EquippedItem;

	/**
	 * member methods
	*/
	void CreateVariables();

	/**
	* Network methods
	*/
	void SetMoveState(Protocol::MoveState State);
	void SetObjectInfo(const Protocol::ObjectInfo& Info);
	void SetPosInfo(const Protocol::PosInfo& Info);

	Protocol::MoveState GetMoveState() { return PosInfo->move_state(); }
	Protocol::ObjectInfo* GetObjectInfo() { return ObjectInfo; }
	Protocol::PosInfo* GetPosInfo() { return PosInfo; }

	/**
	* member variables
	*/
	class Protocol::ObjectInfo* ObjectInfo;
	class Protocol::PosInfo* PosInfo;
};
