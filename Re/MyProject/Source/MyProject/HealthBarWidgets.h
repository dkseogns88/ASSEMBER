// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "HealthBarWidgets.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UHealthBarWidgets : public UUserWidget
{

	GENERATED_BODY()

public:

    virtual void NativeConstruct() override;

    void UpdateHealth(float HealthPercentage);

   

protected:

    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UProgressBar* HealthProgressBar;

    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* HealthText;
	
};
