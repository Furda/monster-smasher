// Fill out your copyright notice in the Description page of Project Settings.



#include "MSAbilitySystemComponent.h"

#include "AbilitySystemGlobals.h"
#include "Systems/GAS/Abilities/MSGameplayAbility.h"
#include "Input/FMSInputAction.h"
#include "Input/MSInputConfig.h"

class UMSGameplayAbility;


// ==============================
// Basic set up and overrides
// ==============================

UMSAbilitySystemComponent::UMSAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}


// ==============================
// Input abilities events
// ==============================

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


// ==============================
// Granting abilities
// ==============================

void UMSAbilitySystemComponent::BroadcastAbilitiesChanged() const
{
	OnAbilitiesChanged.Broadcast();
}

// Grants abilities by iterating over the InputConfig's AbilityInputActions
TArray<FGameplayAbilitySpecHandle> UMSAbilitySystemComponent::GiveAbilitiesFromInputConfig(
	const UMSInputConfig* InputConfig, AActor* InOwnerActor)
{
	if (!IsOwnerActorAuthoritative())
	{
		UE_LOG(LogTemp, Error, TEXT("UMSAbilitySystemComponent::GiveAbilitiesFromInputConfig: Only the server can grant abilities!"));
		return TArray<FGameplayAbilitySpecHandle>();
	}
	
	if (!InputConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("UMSAbilitySystemComponent::GiveAbilitiesFromInputConfig: InputConfig is null!"));
		return TArray<FGameplayAbilitySpecHandle>();
	}

	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;
	
	// All abilities in the input config are granted
	for (const FMSInputAction& Action : InputConfig->AbilityInputActions)
	{
		// if no ability class, skip it
		if (!Action.AbilityClass)
		{
			continue;
		}

		// Get current ability parameters
		int32 AbilityInputID = Action.InputID == EAbilityInputID::None ? -1 : static_cast<int32>(Action.InputID);
		FGameplayAbilitySpec NewAbilitySpec(Action.AbilityClass, 1, AbilityInputID, InOwnerActor);

		// Associate tag to ability
		if (Action.InputTag.IsValid()) NewAbilitySpec.GetDynamicSpecSourceTags().AddTag(Action.InputTag);
		
		// Save the ability spec handle to in case it is needed later
		FGameplayAbilitySpecHandle AbilityHandle = GiveAbility(NewAbilitySpec);
		
		GrantedAbilityHandles.Add(AbilityHandle);
	}
	return GrantedAbilityHandles;
}

void UMSAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	
	UE_LOG(LogTemp, Warning, TEXT("UMSAbilitySystemComponent::OnRep_ActivateAbilities"));
	
	bool bHasAbilitiesChanged = false;
	if (LastActivatableAbilities.Num() != ActivatableAbilities.Items.Num())
	{
		bHasAbilitiesChanged = true;
	}
	else
	{
		for (int i = 0; i < LastActivatableAbilities.Num(); i++)
		{
			if (LastActivatableAbilities[i].Ability != ActivatableAbilities.Items[i].Ability)
			{
				bHasAbilitiesChanged = true;
				break; // break the loop since already detected a change
			}
		}
	}
	
	if (bHasAbilitiesChanged)
	{
		LastActivatableAbilities = ActivatableAbilities.Items;
		BroadcastAbilitiesChanged();
	}
}

void UMSAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);
    
	// Increment pending changes
	PendingAbilityChanges++;
    
	// Schedule deferred broadcast (will reset timer if already pending)
	ScheduleDeferredBroadcast();
}

void UMSAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnRemoveAbility(AbilitySpec);
    
	// Increment pending changes
	PendingAbilityChanges++;
    
	// Schedule deferred broadcast
	ScheduleDeferredBroadcast();
}

void UMSAbilitySystemComponent::ScheduleDeferredBroadcast()
{
	if (UWorld* World = GetWorld())
	{
		// Clear any existing timer
		World->GetTimerManager().ClearTimer(DeferredBroadcastTimer);
        
		// Schedule new broadcast for next tick
		World->GetTimerManager().SetTimerForNextTick([this]()
		{
			ExecuteDeferredBroadcast();
		});
	}
}

void UMSAbilitySystemComponent::ExecuteDeferredBroadcast()
{
	if (PendingAbilityChanges > 0)
	{
		PendingAbilityChanges = 0;
		BroadcastAbilitiesChanged();
	}
}
