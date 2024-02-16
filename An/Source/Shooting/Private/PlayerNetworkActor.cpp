// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerNetworkActor.h"
#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
// Sets default values
APlayerNetworkActor::APlayerNetworkActor()
{
	// ��Ʈ��ũ Ȱ��ȭ �� ��Ÿ ����
	SetReplicates(true);
	PrimaryActorTick.bCanEverTick = true;

	// ���̷�Ż �޽� ������Ʈ ���� �� ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/Path/To/Your/Mesh.Mesh'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}

	// ī�޶� �� ���� �� ����
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // ī�޶���� �Ÿ�
	CameraBoom->bUsePawnControlRotation = true;

	// ī�޶� ������Ʈ ���� �� ����
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void APlayerNetworkActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerNetworkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		NetLocation = GetActorLocation();
		NetRotation = GetActorRotation();
	}

}

void APlayerNetworkActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerNetworkActor, NetLocation);
	DOREPLIFETIME(APlayerNetworkActor, NetRotation);

}

// Called to bind functionality to input
void APlayerNetworkActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// �̵� �Է� ���ε�
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerNetworkActor::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerNetworkActor::MoveRight);

	// ȸ�� �Է� ���ε�
	PlayerInputComponent->BindAxis("Turn", this, &APlayerNetworkActor::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerNetworkActor::AddControllerPitchInput);
}

void APlayerNetworkActor::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// �̵� ���� ����
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// ��/�� �̵� ���� ���
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerNetworkActor::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// �̵� ���� ����
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// ��/�� �̵� ���� ���
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}


