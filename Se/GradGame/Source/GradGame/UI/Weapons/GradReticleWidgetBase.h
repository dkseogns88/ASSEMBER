#pragma once

#include "CommonUserWidget.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "GradReticleWidgetBase.generated.h"

/** forward declarations */
class UGradWeaponInstance;
class UGradInventoryItemInstance;

UCLASS(Abstract)
class UGradReticleWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UGradReticleWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void InitializeFromWeapon(UGradWeaponInstance* InWeapon);

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponInitialized();

	/**
	 * WeaponInstance/InventoryInstance를 상태 추적용으로 캐싱 목적
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UGradWeaponInstance> WeaponInstance;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UGradInventoryItemInstance> InventoryInstance;
};