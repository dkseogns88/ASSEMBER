// Fill out your copyright notice in the Description page of Project Settings.


#include "GradGameState.h"
#include "GradGame/GameModes/GradExperienceManagerComponent.h"

AGradGameState::AGradGameState()
{
	ExperienceManagerComponent = CreateDefaultSubobject<UGradExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
