// Fill out your copyright notice in the Description page of Project Settings.


#include "GradEquipmentDefinition.h"
#include "GradEquipmentInstance.h"

UGradEquipmentDefinition::UGradEquipmentDefinition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// 기본값으로, GradEquipmentInstance로 설정
	InstanceType = UGradEquipmentInstance::StaticClass();
}
