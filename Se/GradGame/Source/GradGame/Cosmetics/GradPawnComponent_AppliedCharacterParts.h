// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GradCharacterPartTypes.h"
#include "GradCosmeticAnimationTypes.h"
#include "Components/PawnComponent.h"
#include "GradPawnComponent_AppliedCharacterParts.generated.h" 

class UGradPawnComponent_AppliedCharacterParts;

/** 인스턴스화 된 Character Part의 단위 */
USTRUCT()
struct FGradAppliedCharacterPartEntry
{
	GENERATED_BODY()

	/** Character Part의 정의(메타 데이터) */
	UPROPERTY()
	FGradCharacterPart Part;

	/** GradCharacterPartList에서 할당 받은 Part 핸들값 (FGradControllerCharacterPartEntry의 Handle 값과 같아야 함 -> 같으면 같은 Part) */
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	/** 인스턴스화 된 Character Part용 Actor */
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

/** GradPawnComponent_CharacterParts에서 실질적 Character Parts를 관리하는 클래스 */
USTRUCT(BlueprintType)
struct FGradCharacterPartList
{
	GENERATED_BODY()

	FGradCharacterPartList()
		: OwnerComponent(nullptr)
	{}

	FGradCharacterPartList(UGradPawnComponent_AppliedCharacterParts* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{}

	bool SpawnActorForEntry(FGradAppliedCharacterPartEntry& Entry);
	void DestroyActorForEntry(FGradAppliedCharacterPartEntry& Entry);

	FGradCharacterPartHandle AddEntry(FGradCharacterPart NewPart);
	void RemoveEntry(FGradCharacterPartHandle Handle);

	FGameplayTagContainer CollectCombinedTags() const;

	/** 현재 인스턴스화된 Character Part */
	UPROPERTY()
	TArray<FGradAppliedCharacterPartEntry> Entries;

	/** 해당 GradCharacterPartList의 Owner인 PawnComponent */
	UPROPERTY()
	TObjectPtr<UGradPawnComponent_AppliedCharacterParts> OwnerComponent;

	/** 앞서 보았던 PartHandle의 값을 할당 및 관리하는 변수 */
	int32 PartHandleCounter = 0;
};

/**
 * 
 */
UCLASS()
class GRADGAME_API UGradPawnComponent_AppliedCharacterParts : public UPawnComponent
{
	GENERATED_BODY()
public:
	UGradPawnComponent_AppliedCharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	USkeletalMeshComponent* GetParentMeshComponent() const;
	USceneComponent* GetSceneComponentToAttachTo() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Cosmetics)
	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;
	void BroadcastChanged();

	FGradCharacterPartHandle AddCharacterPart(const FGradCharacterPart& NewPart);
	void RemoveCharacterPart(FGradCharacterPartHandle Handle);

	/** 인스턴스화 된 Character Parts */
	UPROPERTY()
	FGradCharacterPartList CharacterPartList;

	/** 애니메이션 적용을 위한 메시와 연결고리 */
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FGradAnimBodyStyleSelectionSet BodyMeshes;
};
