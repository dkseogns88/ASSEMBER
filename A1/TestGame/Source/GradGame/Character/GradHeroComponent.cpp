// Fill out your copyright notice in the Description page of Project Settings.


#include "GradHeroComponent.h"
#include "GradPawnData.h"
#include "GradPawnExtensionComponent.h"
#include "PlayerMappableInputConfig.h"
#include "GradGame/Input/GradMappableConfigPair.h"
#include "GradGame/Input/GradInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GradGame/GradGameplayTags.h"
#include "GradGame/GradLogChannels.h"
#include "GradGame/Camera/GradCameraComponent.h"
#include "GradGame/Player/GradPlayerController.h"
#include "GradGame/Player/GradPlayerState.h"

/** FeatureName 정의: static member variable 초기화 */
const FName UGradHeroComponent::NAME_ActorFeatureName("Hero");

/** InputConfig의 GameFeatureAction 활성화 ExtensioEvent 이름 */
const FName UGradHeroComponent::NAME_BindInputsNow("BindInputsNow");

UGradHeroComponent::UGradHeroComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UGradHeroComponent::OnRegister()
{
	Super::OnRegister();

	// 올바른 Actor에 등록되었는지 확인:
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogGrad, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
			return;
		}
	}

	RegisterInitStateFeature();
}

void UGradHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// PawnExtensionComponent에 대해서 (PawnExtension Feature) OnActorInitStateChanged() 관찰하도록 (Observing)
	BindOnActorInitStateChanged(UGradPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// InitState_Spawned로 초기화
	ensure(TryToChangeInitState(FGradGameplayTags::Get().InitState_Spawned));

	// ForceUpdate 진행
	CheckDefaultInitialization();
}


void UGradHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UGradHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FGradGameplayTags& InitTags = FGradGameplayTags::Get();

	if (Params.FeatureName == UGradPawnExtensionComponent::NAME_ActorFeatureName)
	{
		// GradPawnExtensionComponent의 DataInitialized 상태 변화 관찰 후, GradHeroComponent도 DataInitialized 상태로 변경
		// - CanChangeInitState 확인
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UGradHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	const FGradGameplayTags& InitTags = FGradGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	AGradPlayerState* GradPS = GetPlayerState<AGradPlayerState>();

	// Spawned 초기화
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		if (!GradPS)
		{
			return false;
		}

		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// PawnExtensionComponent가 DataInitialized될 때까지 기다림 (== 모든 Feature Component가 DataAvailable인 상태)
		return GradPS && Manager->HasFeatureReachedInitState(Pawn, UGradPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UGradHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	const FGradGameplayTags& InitTags = FGradGameplayTags::Get();

	// DataAvailable -> DataInitialized 단계
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AGradPlayerState* GradPS = GetPlayerState<AGradPlayerState>();
		if (!ensure(Pawn && GradPS))
		{
			return;
		}

		// Input과 Camera에 대한 핸들링... (TODO)

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const UGradPawnData* PawnData = nullptr;
		if (UGradPawnExtensionComponent* PawnExtComp = UGradPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UGradPawnData>();
		}

		if (bIsLocallyControlled && PawnData)
		{
			// 현재 GradCharacter에 Attach된 CameraComponent를 찾음
			if (UGradCameraComponent* CameraComponent = UGradCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}

		if (AGradPlayerController* GradPC = GetController<AGradPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
	}
}

void UGradHeroComponent::CheckDefaultInitialization()
{
	// 앞서 BindOnActorInitStateChanged에서 보았듯이 Hero Feature는 Pawn Extension Feature에 종속되어 있으므로, CheckDefaultInitializationForImplementers 호출하지 않음:

	// ContinueInitStateChain은 앞서 PawnExtComponent와 같음
	const FGradGameplayTags& InitTags = FGradGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	ContinueInitStateChain(StateChain);
}

PRAGMA_DISABLE_OPTIMIZATION
TSubclassOf<UGradCameraMode> UGradHeroComponent::DetermineCameraMode() const
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (UGradPawnExtensionComponent* PawnExtComp = UGradPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UGradPawnData* PawnData = PawnExtComp->GetPawnData<UGradPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}
PRAGMA_ENABLE_OPTIMIZATION


void UGradHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	// LocalPlayer를 가져오기 위해
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	// EnhancedInputLocalPlayerSubsystem 가져오기 위해
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	// EnhancedInputLocalPlayerSubsystem에 MappingContext를 비워준다:
	Subsystem->ClearAllMappings();

	// PawnExtensionComponent -> PawnData -> InputConfig 존재 유무 판단:
	if (const UGradPawnExtensionComponent* PawnExtComp = UGradPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UGradPawnData* PawnData = PawnExtComp->GetPawnData<UGradPawnData>())
		{
			if (const UGradInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FGradGameplayTags& GameplayTags = FGradGameplayTags::Get();

				// HeroComponent 가지고 있는 Input Mapping Context를 순회하며, EnhancedInputLocalPlayerSubsystem에 추가한다
				for (const FGradMappableConfigPair& Pair : DefaultInputConfigs)
				{
					if (Pair.bShouldActivateAutomatically)
					{
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;

						// 내부적으로 Input Mapping Context를 추가한다:
						// - AddPlayerMappableConfig를 간단히 보는 것을 추천
						Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);
					}
				}

				UGradInputComponent* GradIC = CastChecked<UGradInputComponent>(PlayerInputComponent);
				{
					// InputTag_Move와 InputTag_Look_Mouse에 대해 각각 Input_Move()와 Input_LookMouse() 멤버 함수에 바인딩시킨다:
					// - 바인딩한 이후, Input 이벤트에 따라 멤버 함수가 트리거된다
					GradIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
					GradIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Completed, this, &ThisClass::Input_Move, false);
					GradIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false);
				}
			}
		}
	}
	// GameFeatureAction_AddInputConfig의 HandlePawnExtension 콜백 함수 전달
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UGradHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();

		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::RightVector);
		const FVector RightDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		Pawn->AddMovementInput(ForwardDirection, Value.X);
		Pawn->AddMovementInput(RightDirection, Value.Y);


		// TODO: 서버에 연결이 안될 경우에는 이걸 실행 하면 안되게 바꾸자.
		if (AGradPlayerController* GradPlayerController = Cast<AGradPlayerController>(Controller))
		{
			GradPlayerController->DesiredInput = Value;

			GradPlayerController->DesiredMoveDirection = FVector::ZeroVector;
			GradPlayerController->DesiredMoveDirection += ForwardDirection * Value.X;
			GradPlayerController->DesiredMoveDirection += RightDirection * Value.Y;
			GradPlayerController->DesiredMoveDirection.Normalize();
			
			GradPlayerController->DesiredYaw = MovementRotation.Yaw;
		}
	}
}
void UGradHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}


	const FVector2D Value = InputActionValue.Get<FVector2D>();
	if (Value.X != 0.0f)
	{
		// X에는 Yaw 값이 있음:
		// - Camera에 대해 Yaw 적용
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		// Y에는 Pitch 값!
		double AimInversionValue = -Value.Y;
		Pawn->AddControllerPitchInput(AimInversionValue);

		if (AGradPlayerController* GradPlayerController = Cast<AGradPlayerController>(Pawn->GetController()))
		{
			GradPlayerController->DesiredPitch = GradPlayerController->GetControlRotation().Pitch;
			//GradPlayerController->DesiredRoll = GradPlayerController->GetControlRotation().Roll;
			//GradPlayerController->DesiredPitch = AimInversionValue;
		}
	}
}
