// Fill out your copyright notice in the Description page of Project Settings.

#include "GradControllerComponent_CharacterParts.h"
#include "GradPawnComponent_CharacterParts.h"

UGradControllerComponent_CharacterParts::UGradControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGradControllerComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (AController* OwningController = GetController<AController>())
		{
			OwningController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
		}
	}
}

void UGradControllerComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();
	Super::EndPlay(EndPlayReason);
}

PRAGMA_DISABLE_OPTIMIZATION
UGradPawnComponent_CharacterParts* UGradControllerComponent_CharacterParts::GetPawnCustomizer() const
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// �����غ���, �츮�� �ռ� GradPawnComponent_CharacterParts�� ��ӹ޴� B_MannequinPawnCosmetics�� �̹� B_Hero_ShooterMannequin�� �߰��Ͽ���.
		// B_MannequinPawnCosmetics�� ��ȯ�Ǳ� ����Ѵ�
		return ControlledPawn->FindComponentByClass<UGradPawnComponent_CharacterParts>();
	}
	return nullptr;
}
PRAGMA_ENABLE_OPTIMIZATION

void UGradControllerComponent_CharacterParts::AddCharacterPart(const FGradCharacterPart& NewPart)
{
	// ���� ���� ������ ��������Ʈ�� �������Ʈ���� ����ؼ� ���� ���̴�.
	AddCharacterPartInternal(NewPart);
}

void UGradControllerComponent_CharacterParts::AddCharacterPartInternal(const FGradCharacterPart& NewPart)
{
	FGradControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	NewEntry.Part = NewPart;

	// �����δ� ���⼭ ���Ϳ� ���� ��ȯ�� ��������� �ʴ´�. ���������� ���� ���� ���� ���⼭ �����Ѵ�.
	if (UGradPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
	}
}


void UGradControllerComponent_CharacterParts::RemoveAllCharacterParts()
{
	if (UGradPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		for (FGradControllerCharacterPartEntry& Entry : CharacterParts)
		{
			PawnCustomizer->RemoveCharacterPart(Entry.Handle);
		}
	}
	CharacterParts.Reset();
}

void UGradControllerComponent_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	// ���� OldPawn�� ���ؼ��� Character Parts�� ���� ����������
	if (UGradPawnComponent_CharacterParts* OldCustomizer = OldPawn ? OldPawn->FindComponentByClass<UGradPawnComponent_CharacterParts>() : nullptr)
	{
		for (FGradControllerCharacterPartEntry& Entry : CharacterParts)
		{
			OldCustomizer->RemoveCharacterPart(Entry.Handle);
			Entry.Handle.Reset();
		}
	}

	// ���ο� Pawn�� ���ؼ� ���� Controller�� ������ �ִ� Character Parts�� �߰�������
	if (UGradPawnComponent_CharacterParts* NewCustomizer = NewPawn ? NewPawn->FindComponentByClass<UGradPawnComponent_CharacterParts>() : nullptr)
	{
		for (FGradControllerCharacterPartEntry& Entry : CharacterParts)
		{
			check(!Entry.Handle.IsValid());
			Entry.Handle = NewCustomizer->AddCharacterPart(Entry.Part);
		}
	}
}

void UGradControllerComponent_CharacterParts::AddCharacterPartNetwork(UGradPawnComponent_CharacterParts* PawnCustomizer, const FGradCharacterPart& NewPart)
{
	FGradControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	NewEntry.Part = NewPart;
	
	if (PawnCustomizer)
	{
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
	}
}