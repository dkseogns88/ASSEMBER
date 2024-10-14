// Fill out your copyright notice in the Description page of Project Settings.


#include "GradAbilitySystemComponent.h"
#include "Abilities/GradGameplayAbility.h"
#include "GameFramework/Pawn.h"
#include "GradGame/Animation/GradAnimInstance.h"

UGradAbilitySystemComponent::UGradAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGradAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	check(ActorInfo);
	check(InOwnerActor);

	const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor);

	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (bHasNewPawnAvatar)
	{
		if (UGradAnimInstance* LyraAnimInst = Cast<UGradAnimInstance>(ActorInfo->GetAnimInstance()))
		{
			LyraAnimInst->InitializeWithAbilitySystem(this);
		}
	}
}

void UGradAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		// ���� GameplayAbilitySpec�� ��ȸ
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			// Ability�� �����ϰ�, DynamicAbilityTags�� InputTag�� ���� ���, InputPressed/Held�� �־� Ability ó���� ����Ѵ�
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UGradAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				// Released�� �߰��ϰ�, Held������ �������ش�
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UGradAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;

	// InputHeldSpecHandles�� ���� Ability ó���� ���� AbilitiesToActivate�� �߰��Ѵ�
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		// FindAbilitySpecFromHandle Ȯ��:
		// - ActivatableAbilities�� Handle �� �񱳸� ���� GameplayAbilitySpec�� ��ȯ�Ѵ�
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const UGradGameplayAbility* GradAbilityCDO = CastChecked<UGradGameplayAbility>(AbilitySpec->Ability);

				// ActivationPolicy�� WhileInputActive �Ӽ��̸� Ȱ��ȭ�� ���
				if (GradAbilityCDO->ActivationPolicy == EGradAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// �̹� Ability�� Ȱ��ȭ�Ǿ� ���� ���, Input Event(InputPressed)�� ȣ��
					// - AbilitySpecInputPressed Ȯ��
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UGradGameplayAbility* GradAbilityCDO = CastChecked<UGradGameplayAbility>(AbilitySpec->Ability);

					// ActivationPolicy�� OnInputTriggered �Ӽ��̸� Ȱ��ȭ�� ���
					if (GradAbilityCDO->ActivationPolicy == EGradAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	// ��ϵ� AbilitiesToActivate�� �Ѳ����� ��� ����:
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		// ��� ���� �� ����Ǿ��ٸ�, CallActivate ȣ��� BP�� Activate ��尡 ����� ����
		TryActivateAbility(AbilitySpecHandle);
	}

	// �̹� �����ӿ� Release�Ǿ��ٸ�, ���� GameplayAbility ó��:
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;
				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	// InputHeldSpecHandles�� InputReleasedSpecHandles �߰��ɶ� ���ŵȴ�!
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}