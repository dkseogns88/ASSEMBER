

#pragma once

#include "CoreMinimal.h"
#include "Character/A1Character.h"
#include "A1Monster.generated.h"

/**
 * 
 */
UCLASS()
class A1_API AA1Monster : public AA1Character
{
	GENERATED_BODY()
	
public:
	AA1Monster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
