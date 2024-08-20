

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "A1PickupInterface.generated.h"

UINTERFACE(MinimalAPI)
class UA1PickupInterface : public UInterface
{
	GENERATED_BODY()
};


class A1_API IA1PickupInterface
{
	GENERATED_BODY()

public:
	virtual void PickupWeapon(FName WeaponName, bool& Success);
	virtual FName GetWeaponName() = 0;
	virtual void GetCamera(FVector& WorldLocation, FVector& ForwardVector) = 0;

};
