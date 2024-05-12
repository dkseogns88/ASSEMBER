// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "AmmoWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UAmmoWidget : public UUserWidget
{

	GENERATED_BODY()
	

public:
    UFUNCTION(BlueprintCallable, Category = "Ammo")
    void UpdateAmmoCount(int32 CurrentAmmo, int32 MaxAmmo);

    virtual void NativeConstruct() override;
    

protected:

  

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CurrentAmmoText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TotalAmmoText;

};
