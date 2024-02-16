// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerNetworkActor.h"
#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
// Sets default values
APlayerNetworkActor::APlayerNetworkActor()
{
	// 네트워크 활성화 및 기타 설정
	SetReplicates(true);
	PrimaryActorTick.bCanEverTick = true;

	// 스켈레탈 메시 컴포넌트 생성 및 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/Path/To/Your/Mesh.Mesh'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}

	// 카메라 붐 생성 및 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // 카메라와의 거리
	CameraBoom->bUsePawnControlRotation = true;

	// 카메라 컴포넌트 생성 및 설정
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

	// 이동 입력 바인딩
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerNetworkActor::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerNetworkActor::MoveRight);

	// 회전 입력 바인딩
	PlayerInputComponent->BindAxis("Turn", this, &APlayerNetworkActor::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerNetworkActor::AddControllerPitchInput);
}

void APlayerNetworkActor::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// 이동 방향 결정
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 전/후 이동 벡터 계산
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerNetworkActor::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// 이동 방향 결정
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 좌/우 이동 벡터 계산
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}


