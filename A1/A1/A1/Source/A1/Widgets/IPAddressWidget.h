// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidget.h"
#include "IPAddressWidget.generated.h"

/**
 * 
 */
UCLASS()
class A1_API UIPAddressWidget : public UBaseWidget
{
	GENERATED_BODY()
	

public:
    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* IPAddressInput;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* IPAddressDisplay;

    UPROPERTY(meta = (BindWidget))
    class UButton* ToggleButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* SubmitButton;

    UFUNCTION()
    void OnIPAddressChanged(const FText& Text);

    UFUNCTION()
    void OnToggleButtonClicked();

    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
private:
    UFUNCTION()
    void OnSubmitButtonClicked();


    bool Connected = false;
};
