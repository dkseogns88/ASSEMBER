

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "A1WeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UA1WeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class A1_API IA1WeaponInterface
{
	GENERATED_BODY()

public:
	virtual void FireON()	= 0;
	virtual void FireOFF()	= 0;

};
