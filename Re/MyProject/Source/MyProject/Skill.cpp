// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Character/Enemy1.h"
#include "Character/Enemy2.h"



USkill::USkill()
{
    SkillName = "DefaultSkill";
    SkillRadius = 3000.0f; // 30m
    SkillDamage = 50.0f;
}

void USkill::Initialize(FName InSkillName, float InSkillRadius, float InSkillDamage)
{
    SkillName = InSkillName;
    SkillRadius = InSkillRadius;
    SkillDamage = InSkillDamage;
}

void USkill::ExecuteSkill(AActor* Instigator)
{
    //Instigator는 스킬을 발동하는 액터
    if (!Instigator)
    {
        return;
    }

    FVector Location = Instigator->GetActorLocation(); //스킬시전자의 위치
    TArray<FOverlapResult> OverlapResults; 
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(SkillRadius); //구모양의 범위설정
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Instigator); //자신은 스킬대상에서 제외
    //범위를 표시하는 구
    DrawDebugSphere(
        Instigator->GetWorld(),
        Location,
        SkillRadius,
        50,
        FColor(255, 0, 0, 128), // 반투명한 빨간색
        false,
        1.0f // 1초 후에 사라짐
    );
    bool bOverlap = Instigator->GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        Location,
        FQuat::Identity,
        ECC_Pawn,
        CollisionShape,
        QueryParams
    );

    if (bOverlap)
    {
        for (auto& OverlapResult : OverlapResults)
        {
            AActor* OverlappedActor = OverlapResult.GetActor();
            if (OverlappedActor && (OverlappedActor->IsA(AEnemy1::StaticClass()) || OverlappedActor->IsA(AEnemy2::StaticClass())))
            {
                UGameplayStatics::ApplyDamage(OverlappedActor, SkillDamage, Instigator->GetInstigatorController(), Instigator, UDamageType::StaticClass()); // 데미지를 가함
                UE_LOG(LogTemp, Log, TEXT("Hit enemy: %s"), *OverlappedActor->GetName());

               
            }
        }
    }
}