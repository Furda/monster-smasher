// Fill out your copyright notice in the Description page of Project Settings.

#include "MSInputConfig.h"
#include "GameplayTagsManager.h"

// Find the Input Action associated with a given tag
const UInputAction* UMSInputConfig::FindInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FMSInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Input Action for InputTag [%s] on InputConfig [%s]."),
		       *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
