

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Network/Protocol.pb.h"
#include "A1Character.generated.h"

UCLASS()
class A1_API AA1Character : public ACharacter
{
	GENERATED_BODY()

public:
	AA1Character();
	virtual ~AA1Character();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


public:
	bool IsMyPlayer();

	//void PlayZoom(bool IsZoom);

	Protocol::MoveState GetMoveState();
	void SetMoveState(Protocol::MoveState State);

public:
	void SetPlayerInfo(const Protocol::PosInfo& Info);
	void SetDestInfo(const Protocol::PosInfo& Info);
	Protocol::PosInfo* GetPlayerInfo();

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Hp = 80;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHp = 100;

	class Protocol::PosInfo* PlayerInfo;	// 현재 위치
	class Protocol::PosInfo* DestInfo;		// 목적지

};
