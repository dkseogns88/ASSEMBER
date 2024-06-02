// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimSequence.h"
#include "Components/BoxComponent.h"
#include "Network/Protocol.pb.h"
#include "NPC.generated.h"

UCLASS()
class MYPROJECT_API ANPC : public ACharacter
{
    GENERATED_BODY()

    friend class UMyProjectGameInstance;
    friend class UMyProjectPlayerController;
protected:
    // 공통 기능
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // 공격 기능
    UFUNCTION(BlueprintCallable, Category = "Attack")
    virtual void Attack();

    // 데미지 기능
    UFUNCTION(BlueprintCallable, Category = "Damage")
    virtual void TakeDamaged();

    // 죽음 기능
    UFUNCTION(BlueprintCallable, Category = "Death")
    virtual void Die();

    void ResetAttack();
    void ResetDamage();
    void HandleDeath();

    // 타이머 핸들러
    FTimerHandle AttackResetTimerHandle;
    FTimerHandle DamageResetTimerHandle;
    FTimerHandle DeathHandle;

    // 상태 플래그
    UPROPERTY(BlueprintReadWrite, Category = "Attack")
    bool bIsAttacking;

    UPROPERTY(BlueprintReadWrite, Category = "Damage")
    bool bIsDamaged;

    UPROPERTY(BlueprintReadWrite, Category = "Death")
    bool bIsDead;

    // 기타 속성
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Info")
    FString NPCName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Info")
    float Health;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BoxComponent;


public:
    ANPC();
    virtual ~ANPC();
    FString GetNPCName() const { return NPCName; }
    float GetHealth() const { return Health; }
    TArray<FVector> GetBoxCornerPoints() const;
    void CheckMeshSetup();
    void CheckAndTeleport();

public:
    Protocol::MoveState GetMoveState() { return MonsterInfo->state(); }
    void SetMoveState(Protocol::MoveState State);

    void SetMonsterInfo(const Protocol::PosInfo& Info);
    void SetDestInfo(const Protocol::PosInfo& Info);
    Protocol::PosInfo* GetMonsterInfo() { return MonsterInfo; }


    float TimeSinceLastAttack;
    const float AttackInterval = 3.0f;  // 3 seconds interval

    class Protocol::PosInfo* MonsterInfo;
    class Protocol::PosInfo* DestInfo;

};
