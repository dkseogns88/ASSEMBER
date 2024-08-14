

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Network/Protocol.pb.h"
#include "Components/TimelineComponent.h"
#include "../A1Types.h"
#include "../Pickup/A1PickupInterface.h"
#include "A1Character.generated.h"

class UA1WeaponComponent;

UCLASS()
class A1_API AA1Character : public ACharacter, public IA1PickupInterface
{
	GENERATED_BODY()

public:
	AA1Character();
	virtual ~AA1Character();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:

    UFUNCTION()
    void CrouchTimelineProgress(float Value);

    // Pickup
    virtual void PickupWeapon(FName WeaponName, bool& Success) override;
    virtual FName GetWeaponName() override;
    
    UFUNCTION(BlueprintCallable)
    void AttachToHand(EOverlayStates Overlay, TSoftClassPtr<AActor> BP_Actor, FTransform SocketTransform);

public:
    // Crouch
    UPROPERTY()
    UTimelineComponent* CrouchTimeline;

    UPROPERTY(EditAnywhere, Category = "Timeline")
    UCurveFloat* CrouchCurve;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UChildActorComponent* HandObject;
    
    Protocol::LocomotionState BeforeLocomotion_State = Protocol::LocomotionState::LOCOMOTION_STATE_IDLE;

public:
    // TODO: 다른 곳으로 옮길 것
    const float WalkSpeed = 310.f;
    const float RunSpeed = 525.f;

    float PlayerWalkSpeed = WalkSpeed;

public:
    bool IsMyPlayer();
    Protocol::MoveState GetMoveState() { return PosInfo->move_state(); }
    void SetMoveState(Protocol::MoveState State);

    Protocol::LocomotionState GetLocomotionState() { return StateInfo->locomotion_state(); }
    void SetLocomotionState(Protocol::LocomotionState State);

    Protocol::OverlayState GetOverlayState() { return StateInfo->overlay_state(); }
    void SetOverlayState(Protocol::OverlayState State);

    void SetObjectInfo(const Protocol::ObjectInfo& Info);
    void SetPosInfo(const Protocol::PosInfo& Info);
    void SetDestInfo(const Protocol::PosInfo& Info);

    Protocol::ObjectInfo* GetObjectInfo() { return ObjectInfo; }
    Protocol::PosInfo* GetPosInfo() { return PosInfo; }

    void SetStatInfo(const Protocol::StatInfo& Info);
    Protocol::StatInfo* GetStatInfo() { return StatInfo; }

    void SetStateInfo(const Protocol::StateInfo& Info);
    Protocol::StateInfo* GetStateInfo() { return StateInfo; }

public:
    bool bCrouch = false;
    
    // TODO: UPROPERTY 없애야 하나?
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    EOverlayStates OverlayState = EOverlayStates::Default;

    bool bAiming = false;
    float AimPitch = 0.f;
    float AimYaw = 0.f;

    // 이거 하나로 합치고 싶은데 해결 못함
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UA1WeaponComponent* WeaponComponent;
    UPROPERTY(EditAnywhere)
    TSubclassOf<UA1WeaponComponent> ObjectWeaponComponent;


protected:
    class Protocol::ObjectInfo* ObjectInfo;

    class Protocol::PosInfo* PosInfo;
    class Protocol::PosInfo* DestInfo;
    class Protocol::StatInfo* StatInfo;
    class Protocol::StateInfo* StateInfo;

};
