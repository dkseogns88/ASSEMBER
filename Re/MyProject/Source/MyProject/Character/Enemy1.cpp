// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy1.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AEnemy1::AEnemy1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    NPCName = TEXT("Enemy 1 : Cultist");
    Health = 100.0f;

    // Create and initialize the skeletal mesh component
    USkeletalMeshComponent* SkeletalMesh = GetMesh();
    if (!SkeletalMesh)
    {
        SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
        SetRootComponent(SkeletalMesh);
    }
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Asset/Enemy01/ENEMY01.ENEMY01"));

    if (MeshAsset.Succeeded())
    {
        SkeletalMesh->SetSkeletalMesh(MeshAsset.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load skeletal mesh."));
    }

    SkeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));
    SkeletalMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
    SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SkeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
    SkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
    SkeletalMesh->SetVisibility(true, true);

    BoxComponent->SetupAttachment(RootComponent);
    BoxComponent->SetBoxExtent(FVector(25.0f, 25.0f, 100.0f));
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
    BoxComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));

    static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> AnimBP(TEXT("AnimBlueprintGeneratedClass'/Game/AnimationBlueprint/Enemy1.Enemy1_C'"));
    if (AnimBP.Succeeded())
    {
        SkeletalMesh->SetAnimInstanceClass(AnimBP.Object);
        GetCharacterMovement()->bRunPhysicsWithNoController = true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load animation blueprint class."));
    }

}

AEnemy1::~AEnemy1()
{

}

void AEnemy1::BeginPlay()
{
    Super::BeginPlay();
    GetMesh()->SetVisibility(true);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CheckMeshSetup();
}

void AEnemy1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //CheckAndTeleport();

    {
        const Protocol::MoveState State = DestInfo->state();

        if (State == Protocol::MOVE_STATE_RUN)
        {
            FVector NowLocation = GetActorLocation();
            FVector NewLocation = FVector(DestInfo->x(), DestInfo->y(), DestInfo->z());

            //float DistanceToDestination = FVector::Dist(NowLocation, NewLocation);

            //// 목적지에 도달했는지 확인
            //if (DistanceToDestination < 10.f)
            //{
            //    return;
            //}

            FVector ForwardDirection = (NewLocation - NowLocation);
            ForwardDirection.Normalize();

            FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(NowLocation, NowLocation + ForwardDirection);

            FRotator NowRotation = GetActorRotation();
            FRotator TargetRotation = FRotator(0, Rotator.Yaw, 0);
            FRotator NewRotation = FMath::RInterpTo(NowRotation, TargetRotation, DeltaTime, 5.f); // 보간 속도는 5.0f로 설정
            SetActorRotation(NewRotation);

            AddMovementInput(ForwardDirection);
        }
    }
}