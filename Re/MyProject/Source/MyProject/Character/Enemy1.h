// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC.h"
#include "Network/Protocol.pb.h"
#include "Enemy1.generated.h"



UCLASS()
class MYPROJECT_API AEnemy1 : public ANPC
{

	GENERATED_BODY()


public:
    AEnemy1();
    virtual ~AEnemy1();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
	class Protocol::PosInfo* MonsterInfo;
	class Protocol::PosInfo* DestInfo;

};
