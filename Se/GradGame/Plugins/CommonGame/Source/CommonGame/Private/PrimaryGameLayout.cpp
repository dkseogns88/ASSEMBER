#include "PrimaryGameLayout.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

UPrimaryGameLayout::UPrimaryGameLayout(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

UCommonActivatableWidgetContainerBase* UPrimaryGameLayout::GetLayerWidget(FGameplayTag LayerName)
{
	return Layers.FindRef(LayerName);
}

void UPrimaryGameLayout::RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget)
{
	if (!IsDesignTime())
	{
		LayerWidget->SetTransitionDuration(0.0);
		Layers.Add(LayerTag, LayerWidget);
	}
}