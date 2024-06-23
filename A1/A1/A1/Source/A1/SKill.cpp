// Fill out your copyright notice in the Description page of Project Settings.


#include "SKill.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Characters/PlayerChar.h"
#include "Characters/Monster.h"
#include "Network/A1NetworkManager.h"


ASKill::ASKill()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}

void ASKill::BeginPlay()
{
	Super::BeginPlay();

  

}

void ASKill::InitializeSkill(AActor* SkillCaster, float SkillRadius, float DamageAmount)
{
	Caster = SkillCaster;
	Radius = SkillRadius;
	Damage = DamageAmount;

	
    ApplyDamage();
    EndSkill();
}


void ASKill::ApplyDamage()
{
    

    if (!Caster) {
       
        return;
    }

    TArray<AActor*> OverlappingActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonster::StaticClass(), OverlappingActors);

    //서버충돌처리
    for (AActor* Actor : OverlappingActors)
    {
        if (Actor != Caster && (Actor->GetActorLocation() - Caster->GetActorLocation()).Size() <= Radius)
        {
            AMonster* HitMonster = Cast<AMonster>(Actor);
            if (HitMonster)
            {
                HitMonster->TakeDMG(Damage);

                APlayerChar* MyCharacter = Cast<APlayerChar>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

                Protocol::C_ATTACK AttackPkt;
                Protocol::AttackInfo* Info = AttackPkt.mutable_info();
                Info->set_attack_object_id(MyCharacter->GetPosInfo()->object_id());
                Info->set_hit_object_id(HitMonster->GetPosInfo()->object_id());
                Info->set_attack_type(Protocol::AttackType::ATTACK_TYPE_SKILL);
                Info->set_skill_type(Protocol::SkillType::SKILL_TYPE_RINTY);

                MyCharacter->GetNetworkManager()->SendPacket(AttackPkt);
            }
        }
    }
}
void ASKill::EndSkill()
{
   
    APlayerChar* PlayerChar = Cast<APlayerChar>(Caster);
    if (PlayerChar)
    {
        PlayerChar->bIsUsingSkill = false;
    }

    if (OnSkillEnd.IsBound())
    {
        OnSkillEnd.Broadcast();
    }

    Destroy();
}