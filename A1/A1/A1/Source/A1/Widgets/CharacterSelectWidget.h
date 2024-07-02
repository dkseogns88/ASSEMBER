// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BaseWidget.h"
#include "CharacterSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class A1_API UCharacterSelectWidget : public UBaseWidget
{
    GENERATED_BODY()

public:
    UCharacterSelectWidget(const FObjectInitializer& ObjectInitializer);

   

    UFUNCTION(BlueprintCallable)
    void RintyButtonClicked();

    UFUNCTION(BlueprintCallable)
    void SidaButtonClicked();

    UFUNCTION(BlueprintCallable)
    void CloseButtonClicked();


protected:

    virtual void NativeConstruct() override;

    void NativeDestruct();

    UPROPERTY(meta = (BindWidget))
    class UImage* RintyImage;

    UPROPERTY(meta = (BindWidget))
    class UImage* SidaImage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TimerText;

    UPROPERTY(meta = (BindWidget))
    class UButton* RintyButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* SidaButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* CloseButton;

 
private:
    void UpdateTimer();

    class AA1PlayerController* PlayerController;
    

    FTimerHandle TimerHandle;
};