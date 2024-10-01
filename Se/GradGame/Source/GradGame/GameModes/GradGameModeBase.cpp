// Fill out your copyright notice in the Description page of Project Settings.


#include "GradGameModeBase.h"
#include "GradExperienceManagerComponent.h"
#include "GradGameState.h"
#include "GradGame/GradLogChannels.h"
#include "GradGame/Character/GradCharacter.h"
#include "GradGame/Player/GradPlayerController.h"
#include "GradGame/Player/GradPlayerState.h"
#include "GradGame/Character/GradPawnData.h"
#include "GradExperienceDefinition.h"
#include "GradGame/Character/GradPawnExtensionComponent.h"
#include "Kismet/GameplayStatics.h"

AGradGameModeBase::AGradGameModeBase()
{
	GameStateClass = AGradGameState::StaticClass();
	PlayerControllerClass = AGradPlayerController::StaticClass();
	PlayerStateClass = AGradPlayerState::StaticClass();
	DefaultPawnClass = AGradCharacter::StaticClass();

}

void AGradGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 아직 GameInstance를 통해, 초기화 작업이 진행되므로, 현 프레임에는 Lyra의 Concept인 Experience 처리를 진행할 수 없다:
	// - 이를 처리하기 위해, 한프레임 뒤에 이벤트를 받아 처리를 이어서 진행한다
	// - Experience를 로딩하는 주체는 게임 모드가 아니라 ExperienceManagerComponent다.
	// - 너무 극 초반이라 ExperienceManagerComponent가 스폰이 안돼어서 다음 프레임에 진행하겠다.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void AGradGameModeBase::InitGameState()
{
	Super::InitGameState();

	// Experience 비동기 로딩을 위한 Delegate를 준비한다:
	UGradExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UGradExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// OnExperienceLoaded 등록
	// - 게임 모드는 Experience 로딩이 완료가 되어 있는 시점을 알 수가 없어서
	// - 로딩이 완료가 되면 그 시점부터 OnExperienceLoaded(로딩)을 진행하다.
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnGradExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

UClass* AGradGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	// GetPawnDataForController를 활용하여, PawnData로부터 PawnClass(우리가 설정한 폰)를 유도하자
	if (const UGradPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

PRAGMA_DISABLE_OPTIMIZATION
void AGradGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

APawn* AGradGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	// - 최종적으로 폰이 월드에 소환

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			// FindPawnExtensionComponent 구현
			if (UGradPawnExtensionComponent* PawnExtComp = UGradPawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				if (const UGradPawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
			}

			SpawnedPawn->FinishSpawning(SpawnTransform);
			return SpawnedPawn;
		}
	}

	return nullptr;
}
PRAGMA_ENABLE_OPTIMIZATION

void AGradGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	// 이 함수는 데디케이스 서버가 맞는지 아니면 개발자의 옵션 설정 뿐만 아니라
	// 에픽게임즈에서 진행하는 매칭 서비스 등 그런 것들을 전부 검사해서
	// 맞는 애를 선택해주는 함수다.
	// 즉, 무조건 Experience를 로딩을 바로 해주는 게 아니라 HandleMatch가 있는 것이다.
	// 우리는 Experience 로딩을 선택하는 단계로만 쓸 것이다.
	// TODO: 여기서 서버 관련을 넣으면 될 듯

	// 해당 함수에서는 우리가 로딩할 Experience에 대해 PrimaryAssetId를 생성하여, OnMatchAssignmentGiven으로 넘겨준다

	FPrimaryAssetId ExperienceId;

	// precedence order (highest wins)
	// - matchmaking assignment (if present)
	// - default experience

	UWorld* World = GetWorld();

	// 우리가 앞서, URL과 함께 ExtraArgs로 넘겼던 정보는 OptionsString에 저정되어 있다.
	if (!ExperienceId.IsValid() && UGameplayStatics::HasOption(OptionsString, TEXT("Experience")))
	{
		// Experience의 Value를 가져와서, PrimaryAssetId를 생성해준다: 이때, HakExperienceDefinition의 Class 이름을 사용한다
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption(OptionsString, TEXT("Experience"));
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType(UGradExperienceDefinition::StaticClass()->GetFName()), FName(*ExperienceFromOptions));
	}

	// fall back to the default experience
	// 일단 기본 옵션으로 default하게 B_GradDefaultExperience로 설정놓자
	if (!ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("GradExperienceDefinition"), FName("B_GradDefaultExperience"));
		// - 로딩을 한 게 아니다. 스캔 되어있는 거를 설정하기 위해서 가지고 온 것
	}

	// 필자가 이해한 HandleMatchAssignmentIfNotExpectingOne과 OnMatchAssignmentGiven()은 아직 직관적으로 이름이 와닫지 않는다고 생각한다
	// - 후일, 어느정도 Lyra가 구현되면, 해당 함수의 명을 더 이해할 수 있을 것으로 예상한다
	OnMatchAssignmentGiven(ExperienceId);
}

void AGradGameModeBase::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	// 해당 함수는 ExperienceManagerComponent을 활용하여 Experience을 로딩하기 위해, ExperienceManagerComponent의 ServerSetCurrentExperience를 호출한다

	check(ExperienceId.IsValid());

	UGradExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UGradExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	ExperienceManagerComponent->ServerSetCurrentExperience(ExperienceId);
	// - 게임 모드가 Experience를 로딩하는 게 아니라 ExperienceManagerComponent에서 로딩
}

bool AGradGameModeBase::IsExperienceLoaded() const
{
	check(GameState);
	UGradExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UGradExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}

void AGradGameModeBase::OnExperienceLoaded(const UGradExperienceDefinition* CurrentExperience)
{ 
	// PlayerController를 순회하며
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);

		// PlayerController가 Pawn을 Possess하지 않았다면, RestartPlayer를 통해 Pawn을 다시 Spawn한다
		// - 한번 OnPossess를 보도록 하자:
		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

const UGradPawnData* AGradGameModeBase::GetPawnDataForController(const AController* InController) const
{
	// 게임 도중에 PawnData가 오버라이드 되었을 경우, PawnData는 PlayerState에서 가져오게 됨
	if (InController)
	{
		if (const AGradPlayerState* GradPS = InController->GetPlayerState<AGradPlayerState>())
		{
			// GetPawnData 구현
			if (const UGradPawnData* PawnData = GradPS->GetPawnData<UGradPawnData>())
			{
				return PawnData;
			}
		}
	}

	// fall back to the default for the current experience
	// 아직 PlayerState에 PawnData가 설정되어 있지 않은 경우, ExperienceManagerComponent의 CurrentExperience로부터 가져와서 설정
	check(GameState);
	UGradExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UGradExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		// GetExperienceChecked 구현
		const UGradExperienceDefinition* Experience = ExperienceManagerComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}

	// 어떠한 케이스에도 핸들링 안되었으면 nullptr
	return nullptr;
}
