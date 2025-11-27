// Fill out your copyright notice in the Description page of Project Settings.


#include "MSAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbility.h"
#include "Systems/GAS/Abilities/MSGameplayAbility.h"
#include "Input/MSInputConfig.h" // For UMSInputConfig

UMSAbilitySystemComponent::UMSAbilitySystemComponent()
{
	// PrimaryComponentTick.bCanEverTick = true;
	// PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UMSAbilitySystemComponent::AbilityLocalInputPressed(int32 InputID)
{
	// Default implementation (call parent) to keep behavior
	Super::AbilityLocalInputPressed(InputID);
}

void UMSAbilitySystemComponent::AbilityLocalInputReleased(int32 InputID)
{
	// Default implementation (call parent) to keep behavior
	Super::AbilityLocalInputReleased(InputID);
}

// Grants abilities by iterating over the InputConfig's AbilityInputActions
void UMSAbilitySystemComponent::GiveAbilitiesFromInputConfig(const UMSInputConfig* InputConfig, AActor* InOwnerActor)
{
	if (!InputConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("UMSAbilitySystemComponent::GiveAbilitiesFromInputConfig: InputConfig is null!"));
		return;
	}

	// All abilities in the input config are granted
	for (const FMSInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (!Action.AbilityClass)
		{
			continue;
		}

		// Get current ability parameters
		int32 AbilityInputID = Action.InputID == EAbilityInputID::None ? -1 : static_cast<int32>(Action.InputID);
		FGameplayTag AbilityTag = Action.InputTag.IsValid() ? Action.InputTag : FGameplayTag::EmptyTag;

		FGameplayAbilitySpec NewAbilitySpec(Action.AbilityClass, 1, AbilityInputID,
		                                    InOwnerActor);
		// Associate tag to ability
		NewAbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityTag);

		// Save the ability spec handle to in case it is needed later
		FGameplayAbilitySpecHandle AbilityHandle = GiveAbility(NewAbilitySpec);
	}
	UE_LOG(LogTemp, Log,
	       TEXT(
		       "UMSAbilitySystemComponent::GiveAbilitiesFromInputConfig: %d abilities granted from InputConfig."
	       ), GetActivatableAbilities().Num());
}
