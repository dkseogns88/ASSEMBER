// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidget.h"
#include "PlayerStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class A1_API UPlayerStatWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	void UpdateStats(float Health, float Speed, float Attack);
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SpeedText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AttackText;
};
