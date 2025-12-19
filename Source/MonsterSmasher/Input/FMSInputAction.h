#pragma once

#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "AbilityInputID.h"

#include "FMSInputAction.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FMSInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag,Ability"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly)
	EAbilityInputID InputID = EAbilityInputID::None;
};