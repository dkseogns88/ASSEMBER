// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "../Objects/MonsterProjectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "PlayerChar.h"
#include "../Objects/A1PlayerController.h"
#include "Sound/SoundCue.h"
#include "Components/SphereComponent.h" 
#include "Network/A1NetworkManager.h"


AMonster::AMonster()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsAttacking = false;
    bIsDamaged = false;
    bIsDead = false;
    TimeSinceLastAttack = 0.0f;
    AttackDuration = 1.0f;
    SwordDMG = 50.0f;
    
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    BoxComponent->SetupAttachment(RootComponent);

    FireSound = nullptr;
}

AMonster::~AMonster()
{

}
void AMonster::BeginPlay()
{

    Super::BeginPlay();
}

void AMonster::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Attack interval handling
    TimeSinceLastAttack += DeltaTime;
    if (TimeSinceLastAttack >= AttackInterval && !bIsDamaged)
    {
        ReadyAttack(true);
        TimeSinceLastAttack = 0.0f;
    }

    if (Health <= 0)
    {
        ReadyAttack(false);
        Die();
    }

    if (bIsAttacking && !bIsDealPlayer)
    {
        CheckSwordHit();
    }
}

void AMonster::ReadyAttack(bool canattack)
{
    if (canattack && !bIsAttacking && !bIsDamaged)
    {
        bIsAttacking = true;
       
        Attack(canattack);
    }
}

void AMonster::Attack(bool canattack)
{
    if (canattack && !bIsDamaged)
    {


        if (MonsterType == "Gunner")
        {
            FireProjectile();
            if (FireSound)
            {
                UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
            }
        }
        else if (MonsterType == "Warrior")
        {
            SwordAttack();
        }

        GetWorld()->GetTimerManager().SetTimer(AttackResetTimerHandle, this, &AMonster::ResetAttack, AttackDuration, false);
    }
}

void AMonster::TakeDMG(float DMG)
{
    if (!bIsDamaged)
    {
        bIsDamaged = true;
        
        Health = Health - DMG;
        GetWorld()->GetTimerManager().SetTimer(DamageResetTimerHandle, this, &AMonster::ResetDamage, 0.5f, false);
        
    }
}

void AMonster::Die()
{
    if (!bIsDead)
    {
        bIsDead = true;


        

        
        GetWorld()->GetTimerManager().SetTimer(DeathHandle, [this]()
            {
                this->Destroy();
            }, 1.5f, false);
    }
}

void AMonster::TurnLeft(float Value)
{
    AddActorLocalRotation(FRotator(0.0f, -Value, 0.0f));
}

void AMonster::TurnRight(float Value)
{
    AddActorLocalRotation(FRotator(0.0f, Value, 0.0f));
}

void AMonster::ResetAttack()
{
    bIsAttacking = false;
    

    // 일정 시간 후 canattack을 true로 설정하여 다시 공격할 수 있도록 함
    GetWorld()->GetTimerManager().SetTimer(AttackResetTimerHandle, this, &AMonster::EnableAttack, AttackInterval, false);
}

void AMonster::ResetDamage()
{
    bIsDamaged = false;
}

void AMonster::EnableAttack()
{
    ReadyAttack(true);
   
}

void AMonster::FireProjectile()
{

    if (ProjectileClass)
    {
        FVector MuzzleLocation = GetActorLocation() + FVector(0, 0, 50);
        FRotator MuzzleRotation = GetActorRotation();

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;  
        SpawnParams.Instigator = GetInstigator();

        AMonsterProjectile* Projectile = GetWorld()->SpawnActor<AMonsterProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
        if (Projectile)
        {
            FVector LaunchDirection = MuzzleRotation.Vector();
            Projectile->ProjectileMovementComponent->Velocity = LaunchDirection * Projectile->ProjectileMovementComponent->InitialSpeed;

            
            Projectile->CollisionComponent->IgnoreActorWhenMoving(this, true);

            Projectile->MonterId = ObjectInfo->object_id();
        }
    }
    
}

void AMonster::SwordAttack()
{
    
    

    // 공격 시작
    bIsAttacking = true;
    bIsDealPlayer = false;

    // 0.7초 후에 공격 종료
    GetWorld()->GetTimerManager().SetTimer(AttackEndHandle, this, &AMonster::EndSwordAttack, 0.7f, false);
    
}

void AMonster::EndSwordAttack()
{
    bIsAttacking = false;
    
}

void AMonster::CheckSwordHit()
{
    FVector Start = GetActorLocation();
    FVector End = Start + GetActorForwardVector() * 100.0f; 

    TArray<FHitResult> HitResults;
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(50.0f); 

    bool bHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECC_Pawn, CollisionShape);

    if (bHit)
    {
        for (FHitResult HitResult : HitResults)
        {
            AActor* HitActor = HitResult.GetActor();
            //서버충돌처리
            if (HitActor && HitActor->IsA(APlayerChar::StaticClass()))
            {
                APlayerChar* HitCharacter = Cast<APlayerChar>(HitActor);
                if (HitCharacter)
                {
                    AA1PlayerController* PlayerController = Cast<AA1PlayerController>(HitCharacter->GetController());
                    if (PlayerController)
                    {
                        PlayerController->ApplyDamage(SwordDMG);
                        bIsDealPlayer = true;


                        Protocol::C_ATTACK AttackPkt;
                        Protocol::AttackInfo* Info = AttackPkt.mutable_info();
                        Info->set_attack_object_id(ObjectInfo->object_id());
                        Info->set_hit_object_id(HitCharacter->GetObjectInfo()->object_id());
                        Info->set_attack_type(Protocol::AttackType::ATTACK_TYPE_BASIC);

                        HitCharacter->GetNetworkManager()->SendPacket(AttackPkt);

                        break;
                    }
                   


                }
            }
        }
    }
}


TArray<FVector> AMonster::GetBoxCornerPoints() const
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



void AMonster::CheckAndTeleport()
{
    FVector CurrentLocation = GetActorLocation();
    if (CurrentLocation.Z < -300.0f)
    {
        FVector NewLocation(0.0f, 0.0f, 300.0f);
        SetActorLocation(NewLocation);
    }
}
