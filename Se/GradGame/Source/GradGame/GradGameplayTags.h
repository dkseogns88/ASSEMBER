#pragma once

#include "Containers/UnrealString.h"
#include "Containers/Map.h"
#include "GameplayTagContainer.h"

/** forward declaration */
class UGameplayTagsManager;

/**
 * HakGameplayTags
 * - singleton containing native gameplay tags
 */
struct FGradGameplayTags
{
	/**
	 * static methods
	 */
	static const FGradGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeTags();

	/**
	 * member methods
	 */
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddAllTags(UGameplayTagsManager& Manager);

	/**
	 * �Ʒ��� GameplayTag�� �ʱ�ȭ ���� �ܰ踦 �ǹ��Ѵ�:
	 * - GameInstance�� �ʱ�ȭ ������ UGameFrameworkComponentManager�� RegisterInitState�� ��ϵǾ� ����������(linear)�ϰ� ������Ʈ �ȴ�
	 * - �� �ʱ�ȭ GameplayTag�� ������ Actor ���̿� �����Ǹ�, GameFrameworkInitStateInterface ��ӹ��� Ŭ������ �ʱ�ȭ ����(Init State)�� ���¸ӽ�(State Machine)�� ���� ���� ������ �������̽��� �����Ѵ�
	 */
	FGameplayTag InitState_Spawned;
	FGameplayTag InitState_DataAvailable;
	FGameplayTag InitState_DataInitialized;
	FGameplayTag InitState_GameplayReady;

	/**
	 * Enhanced Input Tags
	 */
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;

private:
	// static ���� �ʱ�ȭ�� .cpp�� ���ִ� ���� ���� ����!
	static FGradGameplayTags GameplayTags;
};