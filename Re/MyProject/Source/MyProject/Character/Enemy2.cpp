// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy2.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"

// Sets default values
AEnemy2::AEnemy2()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    NPCName = TEXT("Enemy 2 : Fanatic");
    Health = 100.0f;

    // Create and initialize the skeletal mesh component
    USkeletalMeshComponent* SkeletalMesh = GetMesh();
    if (!SkeletalMesh)
    {
        SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
        SetRootComponent(SkeletalMesh);
    }
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Asset/Enemy02/fanatic_animation.fanatic_animation"));


    if (MeshAsset.Succeeded())
    {
        SkeletalMesh->SetSkeletalMesh(MeshAsset.Object);

    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load skeletal mesh."));
    }

    // Setup collision and visibility
    SkeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -160.0f));
    SkeletalMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
    SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SkeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
    SkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
    SkeletalMesh->SetVisibility(true, true);

    // Create and initialize the box component for collision
   
    BoxComponent->SetupAttachment(RootComponent);
    BoxComponent->SetBoxExtent(FVector(25.0f, 25.0f, 100.0f));  // Adjust the size as necessary
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

    // Set the relative location of the box component if necessary
    BoxComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
    // Constructor에서 애니메이션 로드

    static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> AnimBP(TEXT("AnimBlueprintGeneratedClass'/Game/AnimationBlueprint/Enemy2.Enemy2_C'"));
    if (AnimBP.Succeeded())
    {
        SkeletalMesh->SetAnimInstanceClass(AnimBP.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load animation blueprint class."));
    }


}

// Called when the game starts or when spawned
void AEnemy2::BeginPlay()
{
    Super::BeginPlay();
    // 강제로 메쉬의 가시성을 활성화
    GetMesh()->SetVisibility(true);

    // 물리 및 충돌 설정 확인

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
   
 
    CheckMeshSetup();
}

// Called every frame
void AEnemy2::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    CheckAndTeleport();
    
}





