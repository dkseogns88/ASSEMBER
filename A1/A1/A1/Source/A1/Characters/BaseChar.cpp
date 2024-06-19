#include "BaseChar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerChar.h"
#include "OtherPlayerChar.h"


ABaseChar::ABaseChar()
{
 	
	PrimaryActorTick.bCanEverTick = true;
    IsMoving = false;

    GetCharacterMovement()->bRunPhysicsWithNoController = true;
    PlayerInfo = new Protocol::PosInfo();
    DestInfo = new Protocol::PosInfo();
}

ABaseChar::~ABaseChar()
{
    delete PlayerInfo;
    delete DestInfo;
    PlayerInfo = nullptr;
    DestInfo = nullptr;
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
    if (PlayerInfo->state() == State)
        return;

    PlayerInfo->set_state(State);
}

void ABaseChar::SetPlayerInfo(const Protocol::PosInfo& Info)
{
    if (PlayerInfo->object_id() != 0)
    {
        assert(PlayerInfo->object_id() == Info.object_id());
    }

    PlayerInfo->CopyFrom(Info);

    FVector Location(Info.x(), Info.y(), Info.z());
    SetActorLocation(Location);
}

void ABaseChar::SetDestInfo(const Protocol::PosInfo& Info)
{
    if (PlayerInfo->object_id() != 0)
    {
        assert(PlayerInfo->object_id() == Info.object_id());
    }

    DestInfo->CopyFrom(Info);

    SetMoveState(Info.state());
}