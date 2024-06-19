// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SKill.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BombSkill.generated.h"

/**
 * 
 */
UCLASS()
class A1_API ABombSkill : public ASKill
{
	GENERATED_BODY()
	
public:
    ABombSkill();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FVector ImpactLocation;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
    UStaticMeshComponent* BombMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
    UProjectileMovementComponent* ProjectileMovement;
   

    virtual void InitializeSkill(AActor* SkillCaster, FVector TargetLocation, float SkillRadius, float DamageAmount);
    void ThrowBomb(FVector LaunchVelocity);

    virtual void ApplyDamage() override;

private:
   

    FTimerHandle TimerHandle_Damage;

protected:
    virtual void BeginPlay() override;
   

};


