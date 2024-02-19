// Fill out your copyright notice in the Description page of Project Settings.


#include "NewCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"



ANewCharacter::ANewCharacter()

{
	
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent2P = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent2P->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent2P->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent2P->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh2P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh2P"));
	Mesh2P->SetOnlyOwnerSee(true);
	Mesh2P->SetupAttachment(FirstPersonCameraComponent2P);
	Mesh2P->bCastDynamicShadow = false;
	Mesh2P->CastShadow = false;
	//Mesh2P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh2P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetCharacterMovement()->bRunPhysicsWithNoController = true;
	
}

ANewCharacter::~ANewCharacter()
{
	
}



// Called when the game starts or when spawned
void ANewCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
}

void ANewCharacter::Tick(float DeltaSeconds)
{


}



