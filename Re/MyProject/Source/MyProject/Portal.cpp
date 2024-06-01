// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Character/MyProjectPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject.h"


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
        GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("OnOverlapBegin!!")));

        auto* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        AMyProjectPlayer* Player = Cast<AMyProjectPlayer>(PC->GetPawn());

        Protocol::C_TELEPORT TelePortPkt;

        Protocol::PosInfo* Info = TelePortPkt.mutable_info();
        Info->CopyFrom(*Player->GetPlayerInfo());
        
        SEND_PACKET(TelePortPkt);
    }
}
