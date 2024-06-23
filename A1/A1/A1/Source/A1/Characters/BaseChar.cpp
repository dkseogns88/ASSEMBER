#include "BaseChar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerChar.h"
#include "OtherPlayerChar.h"


ABaseChar::ABaseChar()
{
 	
	PrimaryActorTick.bCanEverTick = true;
    IsMoving = false;

    GetCharacterMovement()->bRunPhysicsWithNoController = true;
    ObjectInfo = new Protocol::ObjectInfo();
    PosInfo = new Protocol::PosInfo();
    DestInfo = new Protocol::PosInfo();
    StatInfo = new Protocol::StatInfo();
}

ABaseChar::~ABaseChar()
{
    delete ObjectInfo;
    delete PosInfo;
    delete DestInfo;
    delete StatInfo;
    ObjectInfo = nullptr;
    DestInfo = nullptr;
    StatInfo = nullptr;
    StatInfo = nullptr;
}

void ABaseChar::BeginPlay()
{
	Super::BeginPlay();

    {
        FVector Location = GetActorLocation();
        DestInfo->set_x(Location.X);
        DestInfo->set_y(Location.Y);
        DestInfo->set_z(Location.Z);
        DestInfo->set_yaw(GetControlRotation().Yaw);

        SetMoveState(Protocol::MOVE_STATE_IDLE);
    }
}


void ABaseChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ABaseChar::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorForwardVector(), Value);
        IsMoving = true;
    }
    else
    {
        IsMoving = false;
    }
}

void ABaseChar::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorRightVector(), Value);
        IsMoving = true;
    }
    else
    {
        IsMoving = false;
    }
}


void ABaseChar::Jump()
{
    Super::Jump();
}

void ABaseChar::ReadyAttack(bool canattack)
{
    canattack = true;
    
}

void ABaseChar::Attack(bool canattack)
{
    canattack = false;
}

void ABaseChar::TakeDMG(float Value)
{
    
}

void ABaseChar::Die()
{
  
}

void ABaseChar::TurnLeft(float Value)
{
    AddActorLocalRotation(FRotator(0.0f, -Value, 0.0f));
}

void ABaseChar::TurnRight(float Value)
{
    AddActorLocalRotation(FRotator(0.0f, Value, 0.0f));
}

bool ABaseChar::IsMyPlayer()
{
    return Cast<APlayerChar>(this) != nullptr;

}


void ABaseChar::SetMoveState(Protocol::MoveState State)
{
    if (PosInfo->state() == State)
        return;

    PosInfo->set_state(State);
}

void ABaseChar::SetObjectInfo(const Protocol::ObjectInfo& Info)
{
    if (ObjectInfo->object_id() != 0)
    {
        assert(ObjectInfo->object_id() == Info.object_id());
    }

    ObjectInfo->CopyFrom(Info);
}

void ABaseChar::SetPosInfo(const Protocol::PosInfo& Info)
{
    if (PosInfo->object_id() != 0)
    {
        assert(PosInfo->object_id() == Info.object_id());
    }

    PosInfo->CopyFrom(Info);

    FVector Location(Info.x(), Info.y(), Info.z());
    SetActorLocation(Location);
}

void ABaseChar::SetDestInfo(const Protocol::PosInfo& Info)
{
    if (PosInfo->object_id() != 0)
    {
        assert(PosInfo->object_id() == Info.object_id());
    }

    DestInfo->CopyFrom(Info);

    SetMoveState(Info.state());
}

void ABaseChar::SetStatInfo(const Protocol::StatInfo& Info)
{
    if (StatInfo->object_id() != 0)
    {
        assert(StatInfo->object_id() == Info.object_id());
    }

    StatInfo->CopyFrom(Info);
}
