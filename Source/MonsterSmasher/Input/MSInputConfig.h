// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInputID.h"
#include "MSInputConfig.generated.h"


class UGameplayAbility;
class UInputAction;

USTRUCT(BlueprintType)
struct FMSInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly)
	EAbilityInputID InputID = EAbilityInputID::None;
};

/**
 * UMSInputConfig
 * Defines a set of Input Actions and their associated Gameplay Tags.
 * Useful for mapping inputs to abilities via tags.
 */
UCLASS(BlueprintType, Const)
class MONSTERSMASHER_API UMSInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	// Input Actions to bind and their associated tags
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TArray<FMSInputAction> AbilityInputActions;

	// Optional: Get a specific Input Action by its associated tag
	const UInputAction* FindInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;
};
