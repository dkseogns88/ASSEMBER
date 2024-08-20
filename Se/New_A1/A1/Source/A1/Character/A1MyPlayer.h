

#pragma once

#include "CoreMinimal.h"
#include "Character/A1Character.h"
#include "A1MyPlayer.generated.h"


UCLASS()
class A1_API AA1MyPlayer : public AA1Character
{
	GENERATED_BODY()
	
public:
	AA1MyPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	class UA1NetworkManager* GetNetworkManager() const;

	virtual void GetCamera(FVector& WorldLocation, FVector& ForwardVector) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> TTP_Camera;


public:
	UPROPERTY(BlueprintReadOnly)
	bool bSprint = false;

};
