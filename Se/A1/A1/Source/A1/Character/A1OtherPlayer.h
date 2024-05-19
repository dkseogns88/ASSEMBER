

#pragma once

#include "CoreMinimal.h"
#include "Character/A1Character.h"
#include "A1OtherPlayer.generated.h"

UCLASS()
class A1_API AA1OtherPlayer : public AA1Character
{
	GENERATED_BODY()

public:
	AA1OtherPlayer();
	virtual ~AA1OtherPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


	void PlayZoom(bool IsZoom);

};
