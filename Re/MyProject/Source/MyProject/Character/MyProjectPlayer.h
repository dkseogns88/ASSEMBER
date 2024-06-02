// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Network/Protocol.pb.h"
#include "MyProjectPlayer.generated.h"

UCLASS()
class MYPROJECT_API AMyProjectPlayer : public ACharacter
{
    GENERATED_BODY()

public:
    AMyProjectPlayer();
    virtual ~AMyProjectPlayer();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(ReplicatedUsing = OnRep_RollingChanged)
    bool bIsRolling = false;

    UFUNCTION()
    void OnRep_RollingChanged();
    
public:
    bool IsMyPlayer();
    void SetMovementSpeed(float NewSpeed);
    Protocol::MoveState GetMoveState() { return PlayerInfo->state(); }
    void SetMoveState(Protocol::MoveState State);

    void SetPlayerInfo(const Protocol::PosInfo& Info);
    void SetDestInfo(const Protocol::PosInfo& Info);
    Protocol::PosInfo* GetPlayerInfo() { return PlayerInfo; }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float Speed;


    FVector RollDirection;
    float RollDuration;
    FTimerHandle TimerHandle_Roll;

    bool IsRolling() const { return bIsRolling; }
    void SetRolling(bool bNewRolling);

    void StartRoll();
    void EndRoll();
  
  

protected:
    class Protocol::PosInfo* PlayerInfo;
    class Protocol::PosInfo* DestInfo;

private:
    Protocol::MoveState PreviousState; //이전상태저장하여 점프중복방지
};