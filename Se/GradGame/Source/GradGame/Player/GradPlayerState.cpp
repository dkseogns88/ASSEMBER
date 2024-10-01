// Fill out your copyright notice in the Description page of Project Settings.


#include "GradPlayerState.h"
#include "GradGame/GameModes/GradExperienceManagerComponent.h"
#include "GradGame/GameModes/GradGameModeBase.h"

void AGradPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	UGradExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UGradExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnGradExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void AGradPlayerState::OnExperienceLoaded(const UGradExperienceDefinition* CurrentExperience)
{
	// CurrentExperience가 있는데 왜 게임 모드로 넘어가서 가지고 올까? 2가지 이유
	// 1. 안정성 검사
	// 2. 폰 데이터를 가지고 오는 함수를 하나의 함수로 통일하기 위함
	//   - Experience Load#4 5분쯤 설명

	if (AGradGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AGradGameModeBase>())
	{
		// AGradGameMode에서 GetPawnDataForController를 구현해야 함
		// - GetPawnDataForController에서 우리는 아직 PawnData를 설정하지 않았으므로, ExperienceMangerComponent의 DefaultPawnData로 설정한다
		const UGradPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void AGradPlayerState::SetPawnData(const UGradPawnData* InPawnData)
{
	check(InPawnData);

	// PawnData가 두번 설정되는 것은 원하지 않음!
	check(!PawnData);

	PawnData = InPawnData;
}
