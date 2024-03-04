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


	// Create a Spring Arm Component
	CameraBoom2P = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom2P->SetupAttachment(GetCapsuleComponent());
	CameraBoom2P->TargetArmLength = 300.f; // 카메라와 플레이어 사이의 거리
	CameraBoom2P->bUsePawnControlRotation = true; // 스프링 암이 Pawn의 컨트롤 회전을 따르도록 설정

	// Create a CameraComponent	
	FirstPersonCameraComponent2P = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	// 카메라를 스프링 암에 부착
	FirstPersonCameraComponent2P->SetupAttachment(CameraBoom2P, USpringArmComponent::SocketName);
	FirstPersonCameraComponent2P->bUsePawnControlRotation = false; // 카메라 자체 회전은 사용하지 않음

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh2P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh2P->SetOnlyOwnerSee(true);
	Mesh2P->SetupAttachment(GetCapsuleComponent());
	Mesh2P->bCastDynamicShadow = false;
	Mesh2P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh2P->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

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



