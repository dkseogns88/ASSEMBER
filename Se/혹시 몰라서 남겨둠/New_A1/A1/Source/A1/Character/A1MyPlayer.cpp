#include "Character/A1MyPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Network/A1NetworkManager.h"

AA1MyPlayer::AA1MyPlayer()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->SetRelativeRotation(FRotator(-30.f, 0, 0));
	SpringArm->TargetArmLength = 500.f;
	
	TTP_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("TTP_Camera"));
	TTP_Camera->SetupAttachment(SpringArm);

}

void AA1MyPlayer::BeginPlay()
{
	Super::BeginPlay();

}

void AA1MyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UA1NetworkManager* AA1MyPlayer::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UA1NetworkManager>();

}

void AA1MyPlayer::GetCamera(FVector& WorldLocation, FVector& ForwardVector)
{
	WorldLocation = TTP_Camera->GetComponentLocation();
	ForwardVector = TTP_Camera->GetForwardVector();
}
