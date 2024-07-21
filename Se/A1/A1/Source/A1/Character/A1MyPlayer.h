

#pragma once

#include "CoreMinimal.h"
#include "Character/A1Character.h"
#include "A1MyPlayer.generated.h"


UCLASS()
class A1_API AA1MyPlayer : public AA1Character
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AA1MyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	class UA1NetworkManager* GetNetworkManager() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> Camera;


	float TimeAccumulator = 0.f;
};
