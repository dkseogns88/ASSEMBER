// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BaseWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class A1_API UGameOverWidget : public UBaseWidget
{
    GENERATED_BODY()

public:
    UGameOverWidget(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GameOverText;

    UPROPERTY(meta = (BindWidget))
    class UButton* ExitButton;

    UFUNCTION()
    void OnExitButtonClicked();
};