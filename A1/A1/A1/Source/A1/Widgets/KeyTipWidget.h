// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BaseWidget.h"
#include "KeyTipWidget.generated.h"

/**
 * 
 */
UCLASS()
class A1_API UKeyTipWidget : public UBaseWidget
{
	GENERATED_BODY()


public:
    UKeyTipWidget(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;

    void ToggleVisibility();

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* KeyTipsText;

};
