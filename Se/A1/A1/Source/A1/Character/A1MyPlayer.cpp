#include "Character/A1MyPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Network/A1NetworkManager.h"

AA1MyPlayer::AA1MyPlayer()
{

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->SetRelativeRotation(FRotator(-30.f, 0, 0));
	SpringArm->TargetArmLength = 500.f;
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

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