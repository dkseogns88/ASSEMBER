// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IPAddressWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UIPAddressWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* IPAddressInput;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* IPAddressDisplay;

    UPROPERTY(meta = (BindWidget))
    class UButton* ToggleButton;

    UFUNCTION()
    void OnIPAddressChanged(const FText& Text);

    UFUNCTION()
    void OnToggleButtonClicked();

    virtual void NativeConstruct() override;
};
