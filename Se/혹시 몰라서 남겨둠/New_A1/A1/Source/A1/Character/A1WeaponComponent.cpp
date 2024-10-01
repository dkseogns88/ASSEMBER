


#include "Character/A1WeaponComponent.h"

UA1WeaponComponent::UA1WeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UA1WeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UA1WeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}