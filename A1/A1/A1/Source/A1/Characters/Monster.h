// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseChar.h"
#include "Animation/AnimSequence.h"
#include "Components/BoxComponent.h"
#include "Monster.generated.h"

/**
 * 
 */
UCLASS()
class A1_API AMonster : public ABaseChar
{

    GENERATED_BODY()

    friend class AA1GameInstance;
    friend class AA1PlayerController;
    
protected:
  
    virtual void BeginPlay();
    virtual void Tick(float DeltaTime);
    virtual void ReadyAttack(bool canattack) override;
    virtual void Attack(bool canattack) override;
    
    virtual void Die() override;
    virtual void TurnLeft(float Value) override;
    virtual void TurnRight(float Value) override;
    void ResetAttack();
    void ResetDamage();
    void EnableAttack();
    void FireProjectile();
    void SwordAttack();
    void CheckSwordHit();
    void EndSwordAttack();
 

   

    // 타이머 핸들러
    FTimerHandle AttackResetTimerHandle;
    FTimerHandle DamageResetTimerHandle;
    FTimerHandle DeathHandle;
    FTimerHandle AttackEndHandle;

    // 상태 플래그
    UPROPERTY(BlueprintReadWrite, Category = "Character State")
    bool bIsAttacking = false;

    UPROPERTY(BlueprintReadWrite, Category = "Character State")
    bool bIsDamaged;

    UPROPERTY(BlueprintReadWrite, Category = "Character State")
    bool bIsDead;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
    float AttackDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
    TSubclassOf<class AMonsterProjectile> ProjectileClass;

    // 기타 속성
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Info")
    FString MonName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Info")
    float Health;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BoxComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character State")
    float AttackInterval = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Info")
    FString MonsterType;
    
    bool IsMoving;

    bool bIsDealPlayer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundCue* FireSound;
   


public:
    AMonster();
    virtual ~AMonster();
    FString GetMonsterName() const { return MonName; }
    float GetHealth() const { return Health; }
    TArray<FVector> GetBoxCornerPoints() const;
    void CheckMeshSetup();
    void CheckAndTeleport();
    virtual void TakeDMG(float Value) override;

public:
   

    float TimeSinceLastAttack;
   



	
};
