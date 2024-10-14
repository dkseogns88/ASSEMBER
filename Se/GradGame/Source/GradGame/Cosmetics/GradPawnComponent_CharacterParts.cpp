// Fill out your copyright notice in the Description page of Project Settings.

#include "GradPawnComponent_CharacterParts.h"
#include "GradPawnComponent_AppliedCharacterParts.h"

UGradPawnComponent_CharacterParts::UGradPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGradPawnComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();

	/*if (AController* OwningController = GetController<AController>())
	{
		OwningController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
	}*/
}

void UGradPawnComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();
	Super::EndPlay(EndPlayReason);
}

PRAGMA_DISABLE_OPTIMIZATION
UGradPawnComponent_AppliedCharacterParts* UGradPawnComponent_CharacterParts::GetPawnCustomizer() const
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// 생각해보면, 우리는 앞서 GradPawnComponent_CharacterParts를 상속받는 B_MannequinPawnCosmetics를 이미 B_Hero_ShooterMannequin에 추가하였다.
		// B_MannequinPawnCosmetics를 반환되길 기대한다
		return ControlledPawn->FindComponentByClass<UGradPawnComponent_AppliedCharacterParts>();
	}
	return nullptr;
}
PRAGMA_ENABLE_OPTIMIZATION

void UGradPawnComponent_CharacterParts::AddCharacterPart(const FGradCharacterPart& NewPart)
{
	// 따로 나눈 이유는 데디케이트와 블루프린트에서 사용해서 나눈 것이다.
	AddCharacterPartInternal(NewPart);
}

void UGradPawnComponent_CharacterParts::AddCharacterPartInternal(const FGradCharacterPart& NewPart)
{
	FGradCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	NewEntry.Part = NewPart;

	// 실제로는 여기서 액터에 대한 소환을 담당하지는 않는다. 교육적으로 보기 쉽기 위해 여기서 실행한다.
	if (UGradPawnComponent_AppliedCharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
	}
}


void UGradPawnComponent_CharacterParts::RemoveAllCharacterParts()
{
	if (UGradPawnComponent_AppliedCharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		for (FGradCharacterPartEntry& Entry : CharacterParts)
		{
			PawnCustomizer->RemoveCharacterPart(Entry.Handle);
		}
	}
	CharacterParts.Reset();
}

void UGradPawnComponent_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	// 이전 OldPawn에 대해서는 Character Parts를 전부 제거해주자
	if (UGradPawnComponent_AppliedCharacterParts* OldCustomizer = OldPawn ? OldPawn->FindComponentByClass<UGradPawnComponent_AppliedCharacterParts>() : nullptr)
	{
		for (FGradCharacterPartEntry& Entry : CharacterParts)
		{
			OldCustomizer->RemoveCharacterPart(Entry.Handle);
			Entry.Handle.Reset();
		}
	}

	// 새로운 Pawn에 대해서 기존 Controller가 가지고 있는 Character Parts를 추가해주자
	if (UGradPawnComponent_AppliedCharacterParts* NewCustomizer = NewPawn ? NewPawn->FindComponentByClass<UGradPawnComponent_AppliedCharacterParts>() : nullptr)
	{
		for (FGradCharacterPartEntry& Entry : CharacterParts)
		{
			check(!Entry.Handle.IsValid());
			Entry.Handle = NewCustomizer->AddCharacterPart(Entry.Part);
		}
	}
}