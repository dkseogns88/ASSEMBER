// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidget.h"
#include "EnemyInfoWidget.generated.h"


class UTextBlock;
class UProgressBar;

/**
 * 
 */
UCLASS(BlueprintType)
class A1_API UEnemyInfoWidget : public UBaseWidget
{
	GENERATED_BODY()
	

public:

    UFUNCTION(BlueprintCallable, Category = "Enemy Info")
    void SetEnemyName(FString Name);

    UFUNCTION(BlueprintCallable, Category = "Enemy Info")
    void SetEnemyHealth(float HealthPercent);

   
    virtual void ShowWidget();

    
    virtual void HideWidget();

protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* EnemyNameText;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* EnemyHealthBar;
};
