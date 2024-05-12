// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyInfoWidget.generated.h"


class UTextBlock;
class UProgressBar;

/**
 * 
 */
UCLASS(BlueprintType)
class MYPROJECT_API UEnemyInfoWidget : public UUserWidget
{

	GENERATED_BODY()
	
public:
 
    UFUNCTION(BlueprintCallable, Category = "Enemy Info")
    void SetEnemyName(FString Name);

    UFUNCTION(BlueprintCallable, Category = "Enemy Info")
    void SetEnemyHealth(float HealthPercent);

protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* EnemyNameText;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* EnemyHealthBar;
};

