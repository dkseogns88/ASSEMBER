// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Network/Protocol.pb.h"
#include "BaseChar.generated.h"

UCLASS()
class A1_API ABaseChar : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseChar();
    ~ABaseChar();

protected:
	virtual void BeginPlay() override;
    

public:	
	virtual void Tick(float DeltaTime) override;

    
    UFUNCTION(BlueprintCallable, Category = "Character Actions")
    virtual void MoveForward(float Value);

    UFUNCTION(BlueprintCallable, Category = "Character Actions")
    virtual void MoveRight(float Value);

    
    virtual void Jump() override;

   

    UFUNCTION(BlueprintCallable, Category = "Character Actions")
    virtual void ReadyAttack(bool canattack);
   

    UFUNCTION(BlueprintCallable, Category = "Character Actions")
    virtual void Attack(bool canattack);
   

    UFUNCTION(BlueprintCallable, Category = "Character Actions")
    virtual void TakeDMG(float Value);

    UFUNCTION(BlueprintCallable, Category = "Character Actions")
    virtual void Die();

   
    UFUNCTION(BlueprintCallable, Category = "Character Actions")
    virtual void TurnLeft(float Value);

    UFUNCTION(BlueprintCallable, Category = "Character Actions")
    virtual void TurnRight(float Value);

    UPROPERTY(BlueprintReadWrite, Category = "Character State")
    bool IsMoving;

public:
    bool IsMyPlayer();
    Protocol::MoveState GetMoveState() { return PlayerInfo->state(); }
    void SetMoveState(Protocol::MoveState State);

    void SetPlayerInfo(const Protocol::PosInfo& Info);
    void SetDestInfo(const Protocol::PosInfo& Info);
    Protocol::PosInfo* GetPlayerInfo() { return PlayerInfo; }

    protected:
    class Protocol::PosInfo* PlayerInfo;
    class Protocol::PosInfo* DestInfo;

};
