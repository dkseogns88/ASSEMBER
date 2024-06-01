// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterStatue.h"
#include "Components/BoxComponent.h"
#include "MyProjectPlayerController.h"
#include "Character/MyProjectPlayer.h"
#include "MyProject.h"

// Sets default values
ACharacterStatue::ACharacterStatue()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent);
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACharacterStatue::OnOverlapBegin);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACharacterStatue::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ACharacterStatue::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//캐릭터와 동상이 오버랩되었을때
void ACharacterStatue::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        APawn* PlayerPawn = Cast<APawn>(OtherActor);
        if (PlayerPawn)
        {
            AMyProjectPlayerController* PlayerController = Cast<AMyProjectPlayerController>(PlayerPawn->GetController());
            if (PlayerController)
            {
                PlayerController->SetNearbyStatue(this);
            }
        }
    }
}

//캐릭터와 동상이 떨어졌을때
void ACharacterStatue::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this)
    {
        APawn* PlayerPawn = Cast<APawn>(OtherActor);
        if (PlayerPawn)
        {
            AMyProjectPlayerController* PlayerController = Cast<AMyProjectPlayerController>(PlayerPawn->GetController());
            if (PlayerController)
            {
                PlayerController->SetNearbyStatue(nullptr);
            }
        }
    }
}


//캐릭터와 동상이 상호작용시 캐릭터 변경요청  컨트롤러의 ChangeCharacter함수로
void ACharacterStatue::OnInteract(APlayerController* PlayerController)
{
    if (PlayerController)
    {
        //MyController->ChangeCharacter(CharacterName);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Statue")));
        Protocol::C_SELECT SelectPkt;

        AMyProjectPlayer* CurrentPawn = Cast<AMyProjectPlayer>(PlayerController->GetPawn());
        SelectPkt.set_object_id(CurrentPawn->GetPlayerInfo()->object_id());

        if(CharacterName == TEXT("Rinty"))
            SelectPkt.set_player_type(Protocol::PlayerType::PLAYER_TYPE_RINTY);
        else if(CharacterName == TEXT("Sida"))
            SelectPkt.set_player_type(Protocol::PlayerType::PLAYER_TYPE_SIDA);
        
        SEND_PACKET(SelectPkt);
    }
}