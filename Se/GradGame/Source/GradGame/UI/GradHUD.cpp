#include "GradHUD.h"
#include "Components/GameFrameworkComponentManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GradHUD)

AGradHUD::AGradHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void AGradHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	// GradHUD를 Receiver로 Actor를 추가하자
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AGradHUD::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void AGradHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}
