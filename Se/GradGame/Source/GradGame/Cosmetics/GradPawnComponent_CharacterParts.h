// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GradCharacterPartTypes.h"
#include "Components/ControllerComponent.h"
#include "Components/PawnComponent.h"
#include "GradPawnComponent_CharacterParts.generated.h"

class UGradPawnComponent_AppliedCharacterParts;


USTRUCT()
struct FGradCharacterPartEntry
{
	GENERATED_BODY()

	/** Character Part에 대한 정의(메타 데이터 == MetaData) */
	UPROPERTY(EditAnywhere)
	FGradCharacterPart Part;

	/** Character Part 핸들 (고유값) - Controller가 Possess하고 있는 Pawn에서 생성한(인스턴스) Character Part 핸들값 */
	FGradCharacterPartHandle Handle;
};


UCLASS(meta = (BlueprintSpawnableComponent))
class UGradPawnComponent_CharacterParts : public UPawnComponent //UControllerComponent
{
	GENERATED_BODY()
public:
	UGradPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UGradPawnComponent_AppliedCharacterParts* GetPawnCustomizer() const;

	UFUNCTION(BlueprintCallable, Category = Cosmetics)
	void AddCharacterPart(const FGradCharacterPart& NewPart);

	void AddCharacterPartInternal(const FGradCharacterPart& NewPart);

	void RemoveAllCharacterParts();

	/** UFUNCTION으로 정의하지 않으면 AddDynamic이 되지 않는다! */
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FGradCharacterPartEntry> CharacterParts;
};

