// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Character/MyProjectPlayer.h"

// Sets default values
APortal::APortal()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Initialize the collision box
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetCollisionProfileName("Trigger");
    CollisionBox->SetupAttachment(RootComponent);
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);

    // Initialize the portal mesh
    PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
    PortalMesh->SetupAttachment(CollisionBox);

    // Set teleport destination
    TeleportDestination = FVector(1360.f, -70.f, 40.f); //플레이어 시작지점으로 설정
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Overlap event function
void APortal::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        // Check if the overlapping actor is a player character
        AMyProjectPlayer* Character = Cast<AMyProjectPlayer>(OtherActor); //플레이어 일때만 텔레포트가능
        if (Character)
        {
            // Teleport the player to the specified destination
            Character->SetActorLocation(TeleportDestination);
        }
    }
}
