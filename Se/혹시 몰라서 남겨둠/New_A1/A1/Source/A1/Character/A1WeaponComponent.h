

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../A1Types.h"
#include "A1WeaponComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class A1_API UA1WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UA1WeaponComponent();

protected:
	virtual void BeginPlay() override;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	int32 Give(FName WeaponName);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	FA1Weapons CycleUp();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	FA1Weapons CycleDown();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FA1Weapons> Weapons;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FA1Weapons OutRow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName ActiveWeapon;
};
