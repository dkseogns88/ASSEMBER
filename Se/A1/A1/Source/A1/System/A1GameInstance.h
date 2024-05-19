

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "A1GameInstance.generated.h"


UCLASS()
class A1_API UA1GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UA1GameInstance(const FObjectInitializer& ObjectInitializer);

public:
	virtual void Init() override;
	virtual void Shutdown() override;


public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AA1OtherPlayer> OtherPlayerClass;


	UPROPERTY(EditAnywhere)
	TSubclassOf<class AA1Monster> MonsterClass;

};
