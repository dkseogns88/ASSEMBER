// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidget.h"
#include "CrosshairWidget.generated.h"

/**
 * 
 */
UCLASS()
class A1_API UCrosshairWidget : public UBaseWidget
{
	GENERATED_BODY()
	
	
public:
   
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowCrosshair(bool bIsAiming);

protected:
    
    virtual void NativeConstruct() override;

    
    UPROPERTY(meta = (BindWidget))
    class UImage* CrosshairImage;
};
