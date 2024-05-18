


#include "Data/A1InputData.h"
#include "A1LogChannels.h"

const UInputAction* UA1InputData::FindInputActionByTag(const FGameplayTag& InputTag) const
{
	for (const FA1InputAction& Action : InputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	UE_LOG(LogA1, Error, TEXT("Can't find InputAction for InputTag [%s]"), *InputTag.ToString());

	return nullptr;
}
