// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GradCharacterPartTypes.h"
#include "Components/ControllerComponent.h"
#include "GradControllerComponent_CharacterParts.generated.h"

class UGradPawnComponent_CharacterParts;

/** ControllerComponent�� �����ϴ� Character Parts */
USTRUCT()
struct FGradControllerCharacterPartEntry
{
	GENERATED_BODY()

	/** Character Part�� ���� ����(��Ÿ ������ == MetaData) */
	UPROPERTY(EditAnywhere)
	FGradCharacterPart Part;

	/** Character Part �ڵ� (������) - Controller�� Possess�ϰ� �ִ� Pawn���� ������(�ν��Ͻ�) Character Part �ڵ鰪 */
	FGradCharacterPartHandle Handle;
};


UCLASS(meta = (BlueprintSpawnableComponent))
class UGradControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()
public:
	UGradControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UGradPawnComponent_CharacterParts* GetPawnCustomizer() const;

	UFUNCTION(BlueprintCallable, Category = Cosmetics)
	void AddCharacterPart(const FGradCharacterPart& NewPart);

	void AddCharacterPartInternal(const FGradCharacterPart& NewPart);

	void RemoveAllCharacterParts();

	/** UFUNCTION���� �������� ������ AddDynamic�� ���� �ʴ´�! */
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	/**
	* Network methods
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = Network, meta = (DisplayName = "AddPart"))
	void K2_AddPartsNetwork(UGradPawnComponent_CharacterParts* PawnCustomizer);

	UFUNCTION(BlueprintCallable, Category = Cosmetics)
	void AddCharacterPartNetwork(UGradPawnComponent_CharacterParts* PawnCustomizer, const FGradCharacterPart& NewPart);

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FGradControllerCharacterPartEntry> CharacterParts;
};