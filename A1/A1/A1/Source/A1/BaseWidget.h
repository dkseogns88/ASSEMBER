// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class A1_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
    
    UBaseWidget(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category = "UI")
    virtual void ShowWidget();

    UFUNCTION(BlueprintCallable, Category = "UI")
    virtual void HideWidget();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetUIMode(bool bIsUIMode);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetGameMode();

protected:
    
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    void SetInputMode(bool bIsUIMode);
};
