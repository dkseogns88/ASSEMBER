#include "GradExperienceActionSet.h"
#include "GameFeatureAction.h"

UGradExperienceActionSet::UGradExperienceActionSet()
{
}

#if WITH_EDITORONLY_DATA
void UGradExperienceActionSet::UpdateAssetBundleData()
{
	// 우리는 UpdateAssetBundleData() 코드를 한번 봐야한다
	Super::UpdateAssetBundleData();

	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}
}
#endif