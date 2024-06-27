// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSkill.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "BombSkill.generated.h"

/**
 * 
 */
UCLASS()
class A1_API ABombSkill : public ABaseSkill
{
	GENERATED_BODY()
	
public:
    ABombSkill();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FVector ImpactLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float InitialSpeed = 1000.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
    UStaticMeshComponent* BombMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* ExplosionEffect;

    UPROPERTY(EditAnywhere, Category = "Effects")
    USoundCue* ExplosionSound;

    virtual void InitializeSkill(AActor* SkillCaster, FVector TargetLocation, float SkillRadius, float DamageAmount) override;
    void ThrowBomb(FVector LaunchVelocity);

    UFUNCTION()
    void OnBombImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    virtual void PerformDamage(const FVector& DamageLocation) override;

private:
   

    FTimerHandle TimerHandle_Damage;

protected:
    virtual void BeginPlay() override;
   

};


