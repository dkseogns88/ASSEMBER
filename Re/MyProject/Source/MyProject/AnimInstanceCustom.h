#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceCustom.generated.h"

UCLASS()
class UAnimInstanceCustom : public UAnimInstance
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsAiming;

    void SetAiming(bool bAiming);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsRolling;
    
    void SetRolling(bool bRolling);
};


