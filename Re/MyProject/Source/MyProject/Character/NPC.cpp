// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Character/Enemy1.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANPC::ANPC()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsAttacking = false;
    bIsDamaged = false;
    bIsDead = false;
    TimeSinceLastAttack = 0.0f;
    

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    BoxComponent->SetupAttachment(RootComponent);

    MonsterInfo = new Protocol::PosInfo();
    DestInfo = new Protocol::PosInfo();
}
ANPC::~ANPC()
{
    delete MonsterInfo;
    delete DestInfo;
}


void ANPC::BeginPlay()
{
    Super::BeginPlay();
}

void ANPC::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Attack interval handling
    TimeSinceLastAttack += DeltaTime;
    if (TimeSinceLastAttack >= AttackInterval)
    {
        Attack();
        TimeSinceLastAttack = 0.0f;
    }
}

void ANPC::Attack()
{
    // 공격 로직
    if (!bIsAttacking)
    {

        bIsAttacking = true;

        UE_LOG(LogTemp, Log, TEXT("%s Attack triggered"), *NPCName);
        // Reset attack after 1 second (duration of the attack animation)
        GetWorld()->GetTimerManager().SetTimer(AttackResetTimerHandle, this, &ANPC::ResetAttack, 1.0f, false);
    }
}

void ANPC::TakeDamage()
{
    // 데미지 로직
    if (!bIsDamaged)
    {
        bIsDamaged = true;

        UE_LOG(LogTemp, Log, TEXT("%s Damage taken"), *NPCName);
        // Reset damage after 0.5 seconds (duration of the damage animation)
        GetWorld()->GetTimerManager().SetTimer(DamageResetTimerHandle, this, &ANPC::ResetDamage, 0.5f, false);
    }
}

void ANPC::Die()
{
    // 죽음 로직
    if (!bIsDead)
    {
        bIsDead = true;

        UE_LOG(LogTemp, Log, TEXT("%s died"), *NPCName);

        // Trigger death animation and schedule actor destruction
        GetWorld()->GetTimerManager().SetTimer(DeathHandle, this, &ANPC::HandleDeath, 1.5f, false);
    }
}



void ANPC::HandleDeath()
{
    Destroy();
}

void ANPC::ResetAttack()
{
    bIsAttacking = false;
}

void ANPC::ResetDamage()
{
    bIsDamaged = false;
}


TArray<FVector> ANPC::GetBoxCornerPoints() const
{
    TArray<FVector> Points;
    FVector Extent = BoxComponent->GetScaledBoxExtent();
    FVector Origin = BoxComponent->GetComponentLocation();

    FVector BoxPoints[] = {
        FVector(Extent.X, Extent.Y, Extent.Z),
        FVector(Extent.X, Extent.Y, -Extent.Z),
        FVector(Extent.X, -Extent.Y, Extent.Z),
        FVector(Extent.X, -Extent.Y, -Extent.Z),
        FVector(-Extent.X, Extent.Y, Extent.Z),
        FVector(-Extent.X, Extent.Y, -Extent.Z),
        FVector(-Extent.X, -Extent.Y, Extent.Z),
        FVector(-Extent.X, -Extent.Y, -Extent.Z)
    };

    for (const FVector& Point : BoxPoints)
    {
        Points.Add(Origin + Point);
    }

    return Points;
}

void ANPC::CheckMeshSetup()
{
    USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
    if (!SkeletalMeshComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComponent not found in %s"), *GetName());
        return;
    }

    USkeletalMesh* SkeletalMesh = SkeletalMeshComponent->SkeletalMesh;
    if (!SkeletalMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("No skeletal mesh is set for %s"), *GetName());
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Skeletal Mesh for %s is %s"), *GetName(), *SkeletalMesh->GetName());
    }

    int32 MaterialCount = SkeletalMeshComponent->GetNumMaterials();
    UE_LOG(LogTemp, Log, TEXT("Number of materials on %s: %d"), *GetName(), MaterialCount);

    for (int32 i = 0; i < MaterialCount; ++i)
    {
        UMaterialInterface* Material = SkeletalMeshComponent->GetMaterial(i);
        if (!Material)
        {
            UE_LOG(LogTemp, Warning, TEXT("Material %d on %s is not set"), i, *GetName());
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("Material %d on %s is %s"), i, *GetName(), *Material->GetName());
        }
    }
}

void ANPC::CheckAndTeleport()
{
    FVector CurrentLocation = GetActorLocation();
    if (CurrentLocation.Z < -300.0f)
    {
        FVector NewLocation(0.0f, 0.0f, 300.0f);
        SetActorLocation(NewLocation);
    }
}

void ANPC::SetMoveState(Protocol::MoveState State)
{
    if (MonsterInfo->state() == State)
        return;

    MonsterInfo->set_state(State);
}

void ANPC::SetMonsterInfo(const Protocol::PosInfo& Info)
{
    if (MonsterInfo->object_id() != 0)
    {
        assert(MonsterInfo->object_id() == Info.object_id());
    }

    MonsterInfo->CopyFrom(Info);

    FVector Location(Info.x(), Info.y(), Info.z());
    SetActorLocation(Location);

}

void ANPC::SetDestInfo(const Protocol::PosInfo& Info)
{
    if (MonsterInfo->object_id() != 0)
    {
        assert(MonsterInfo->object_id() == Info.object_id());
    }

    DestInfo->CopyFrom(Info);
    SetMoveState(Info.state());
}
