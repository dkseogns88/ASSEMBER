// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidget.h"
#include "LevelUpWidget.generated.h"

/**
 * 
 */
UCLASS()
class A1_API ULevelUpWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category = "LevelUp")
    void OnOptionSelected(int OptionIndex);

protected:
    virtual void NativeConstruct() override;

private:
    class AA1PlayerController* PlayerController;

};
