#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "A1Types.generated.h"

UENUM(BlueprintType)
enum class EOverlayStates : uint8
{
    Default UMETA(DisplayName = "Default"),
    Pistol  UMETA(DisplayName = "Pistol"),
    Rifle   UMETA(DisplayName = "Rifle")
};

UENUM(BlueprintType)
enum class EItemCategories : uint8
{
    Weapons UMETA(DisplayName = "Weapons"),
};

USTRUCT(BlueprintType)
struct FA1Weapons : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<USkeletalMesh> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Ammo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EOverlayStates Overlay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftClassPtr<AActor> BP_Actor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTransform SocketTransform;
};