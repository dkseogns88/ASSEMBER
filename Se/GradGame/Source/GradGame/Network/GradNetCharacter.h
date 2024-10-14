// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ModularCharacter.h"
#include "GradGame/GradGameplayTags.h"
#include "GradNetCharacter.generated.h"

class UGradAbilitySystemComponent;

UCLASS()
class GRADGAME_API AGradNetCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:
	AGradNetCharacter();

	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "NetOnEquipped"))
	void K2_NetOnEquipped();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void WeaponFire(const FGameplayTag& InputTag);

	UGradAbilitySystemComponent* GetGradAbilitySystemComponent() const { return AbilitySystemComponent; }
public:
	UPROPERTY(VisibleAnywhere, Category = "Grad|NetCharacter")
	TObjectPtr<UGradAbilitySystemComponent> AbilitySystemComponent;

public:
	FTransform WeaponTransform = FTransform();
};
