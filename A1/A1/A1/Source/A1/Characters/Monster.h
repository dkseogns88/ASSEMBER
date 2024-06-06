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

    friend class UA1GameInstance;
    friend class UA1PlayerController;
protected:
  
    virtual void BeginPlay();
    virtual void Tick(float DeltaTime);
    virtual void ReadyAttack(bool canattack) override;
    virtual void Attack(bool canattack) override;
    virtual void TakeDMG(float Value) override;
    virtual void Die() override;
    virtual void TurnLeft(float Value) override;
    virtual void TurnRight(float Value) override;
    void ResetAttack();
    void ResetDamage();
 

   

    // 타이머 핸들러
    FTimerHandle AttackResetTimerHandle;
    FTimerHandle DamageResetTimerHandle;
    FTimerHandle DeathHandle;

    // 상태 플래그
    UPROPERTY(BlueprintReadWrite, Category = "Character State")
    bool bIsAttacking = false;

    UPROPERTY(BlueprintReadWrite, Category = "Character State")
    bool bIsDamaged;

    UPROPERTY(BlueprintReadWrite, Category = "Character State")
    bool bIsDead;

    // 기타 속성
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Info")
    FString MonName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Info")
    float Health;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BoxComponent;

    
    bool IsMoving;



public:
    AMonster();
    virtual ~AMonster();
    FString GetMonsterName() const { return MonName; }
    float GetHealth() const { return Health; }
    TArray<FVector> GetBoxCornerPoints() const;
    void CheckMeshSetup();
    void CheckAndTeleport();

public:
   

    float TimeSinceLastAttack;
    const float AttackInterval = 3.0f;  // 3 seconds interval



	
};
