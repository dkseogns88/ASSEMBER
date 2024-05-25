// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelUpWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ULevelUpWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
    UFUNCTION(BlueprintCallable, Category = "LevelUp")
    void OnOptionSelected(int OptionIndex);

protected:
    virtual void NativeConstruct() override;

private:
    class AMyProjectPlayerController* PlayerController;

};
