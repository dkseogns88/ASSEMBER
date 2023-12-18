// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelUpDemoPlayerController.h"
#include "LevelUpGameInstance.h"

void ALevelUpDemoPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);


    ULevelUpGameInstance* gameInst = Cast<ULevelUpGameInstance>(GetWorld()->GetGameInstance());
    if (gameInst) {
        //UE_LOG(LogTemp, Error, TEXT("ALevelUpDemoPlayerController EndPlay Success!!!"));
        gameInst->Cleanup();
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("ALevelUpDemoPlayerController EndPlay fail!!!!"));
    }
}

void ALevelUpDemoPlayerController::Tick(float DeltaTime)
{
    ULevelUpGameInstance* gameInst = Cast<ULevelUpGameInstance>(GetWorld()->GetGameInstance());
    if (gameInst) {
        if (gameInst->isNewPlayer) {
            gameInst->SpawnCharacter();
            gameInst->isNewPlayer = false;
        }
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("ALevelUpDemoPlayerController EndPlay fail!!!!"));
    }
}
