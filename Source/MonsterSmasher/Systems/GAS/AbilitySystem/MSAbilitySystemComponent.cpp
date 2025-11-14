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

// void UMSAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
//                                               FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
// 	UE_LOG(LogTemp, Log,
// 	       TEXT("UMSAbilitySystemComponent::TickComponent. Owner: %s. Avatar: %s. IsActive: %d. TickEnabled: %d"),
// 	       *GetNameSafe(GetOwner()), // GetOwner() is the direct owner (PlayerState or Character)
// 	       *GetNameSafe(GetAvatarActor()), // GetAvatarActor() is the character using the abilities
// 	       IsActive(),
// 	       PrimaryComponentTick.IsTickFunctionEnabled()
// 	);
//
// 	// Call our custom input processing function from TickComponent
// 	ProcessAbilityInput(DeltaTime, IsNetMode(NM_Standalone) ? false : GetWorld()->IsPaused());
// }


// void UMSAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
// {
// 	Super::AbilitySpecInputPressed(Spec);
//
// 	// This is crucial for WaitInputPress ability tasks to work reliably over network.
// 	// We're sending a replicated event for the input press.
// 	// Lyra uses FPredictionKey from the active ability's CurrentActivationInfo.
// 	// For now, let's use a default if it's not active, or the Spec's ActivationInfo if available
// 	// Best approach is to get the current activation info from the *instance* if active.
//
// 	FPredictionKey PredictionKey = Spec.IsActive() && Spec.GetPrimaryInstance()
// 		                               ? Spec.GetPrimaryInstance()->GetCurrentActivationInfo().
// 		                                      GetActivationPredictionKey()
// 		                               : FPredictionKey();
// 	// Or get from Spec.ActivationInfo if that's still your fallback for non-instanced.
//
// 	InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, PredictionKey);
// }

// void UMSAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
// {
// 	Super::AbilitySpecInputReleased(Spec);
//
// 	FPredictionKey PredictionKey = Spec.IsActive() && Spec.GetPrimaryInstance()
// 		                               ? Spec.GetPrimaryInstance()->GetCurrentActivationInfo().
// 		                                      GetActivationPredictionKey()
// 		                               : FPredictionKey();
//
// 	InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, PredictionKey);
// }

// These are the public functions called from AMSPlayerCharacter's input bindings
// void UMSAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
// {
// 	if (InputTag.IsValid())
// 	{
// 		// Iterate through all activatable abilities to find ones that match this InputTag
// 		for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
// 		{
// 			// If the ability's dynamic tags include the InputTag
// 			if (AbilitySpec.Ability && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
// 			{
// 				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
// 				// Add to the list for processing in ProcessAbilityInput
// 				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle); // Add to the list of continuously held abilities
// 				UE_LOG(LogTemp, Log,
// 				       TEXT(
// 					       "UMSAbilitySystemComponent::AbilityInputTagPressed: Ability pressed: '%s'"
// 				       ), *GetNameSafe(AbilitySpec.Ability));
// 			}
// 		}
// 	}
// }

// void UMSAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
// {
// 	if (InputTag.IsValid())
// 	{
// 		// Iterate through all activatable abilities to find ones that match this InputTag
// 		for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
// 		{
// 			if (AbilitySpec.Ability && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
// 			{
// 				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
// 				// Add to the list for processing in ProcessAbilityInput
// 				InputHeldSpecHandles.RemoveSingle(AbilitySpec.Handle); // Remove from continuously held abilities
// 				UE_LOG(LogTemp, Log,
// 				       TEXT(
// 					       "UMSAbilitySystemComponent::AbilityInputTagReleased: Ability released: '%s'"
// 				       ), *GetNameSafe(AbilitySpec.Ability));
// 			}
// 		}
// 	}
// }

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

// This function is called every tick to process input
// void UMSAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
// {
// 	if (bGamePaused)
// 	{
// 		return;
// 	}
//
// 	// You might add a check here for blocking input, similar to Lyra's TAG_Gameplay_AbilityInputBlocked
// 	// if (HasMatchingGameplayTag(TAG_Gameplay_AbilityInputBlocked))
// 	// {
// 	//     ClearAbilityInput(); // A custom function to clear all input states
// 	//     return;
// 	// }
//
// 	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
// 	AbilitiesToActivate.Reset();
//
// 	// Process all abilities that activate when the input is held.
// 	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
// 	{
// 		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
// 		{
// 			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
// 			{
// 				// Queue ability for activation if its policy is WhileInputActive
// 				const UMSGameplayAbility* MSAbilityCDO = Cast<UMSGameplayAbility>(
// 					AbilitySpec->Ability->GetClass()->GetDefaultObject());
// 				if (MSAbilityCDO && MSAbilityCDO->GetActivationPolicy() == EMSAbilityActivationPolicy::WhileInputActive)
// 				{
// 					AbilitiesToActivate.AddUnique(SpecHandle);
// 					UE_LOG(LogTemp, Log,
// 					       TEXT(
// 						       "UMSAbilitySystemComponent::ProcessAbilityInput: Ability Held: '%s'"
// 					       ), *GetNameSafe(AbilitySpec->Ability));
// 				}
// 			}
// 		}
// 	}
//
// 	// Process all abilities that had their input pressed this frame.
// 	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
// 	{
// 		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
// 		{
// 			if (AbilitySpec->Ability)
// 			{
// 				AbilitySpec->InputPressed = true; // Mark input as pressed on the spec
//
// 				// If the ability is active, we tell the ASC to handle the input event,
// 				// which will internally call our AbilitySpecInputPressed override
// 				// and then InvokeReplicatedEvent.
// 				if (AbilitySpec->IsActive())
// 				{
// 					AbilitySpecInputPressed(*AbilitySpec);
// 					UE_LOG(LogTemp, Log,
// 					       TEXT(
// 						       "UMSAbilitySystemComponent::ProcessAbilityInput: Ability active AND pressed: '%s'"
// 					       ), *GetNameSafe(AbilitySpec->Ability));
// 				}
// 				else
// 				{
// 					// Ability is not active. If its policy is OnInputTriggered (like Jump), queue it for activation.
// 					const UMSGameplayAbility* MSAbilityCDO = Cast<UMSGameplayAbility>(
// 						AbilitySpec->Ability->GetClass()->GetDefaultObject());
// 					if (MSAbilityCDO && MSAbilityCDO->GetActivationPolicy() ==
// 						EMSAbilityActivationPolicy::OnInputTriggered)
// 					{
// 						AbilitiesToActivate.AddUnique(SpecHandle);
// 						UE_LOG(LogTemp, Log,
// 						       TEXT(
// 							       "UMSAbilitySystemComponent::ProcessAbilityInput: Ability pressed: '%s'"
// 						       ), *GetNameSafe(AbilitySpec->Ability));
// 					}
// 				}
// 			}
// 		}
// 	}
//
// 	// Try to activate all the abilities that are from presses and holds.
// 	// We do it all at once so that held inputs don't activate the ability
// 	// and then also send an input event to the ability because of the press.
// 	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
// 	{
// 		TryActivateAbility(AbilitySpecHandle);
// 		UE_LOG(LogTemp, Log,
// 		       TEXT(
// 			       "UMSAbilitySystemComponent::ProcessAbilityInput: Activate Ability: '%s' from AbilitiesToActivate"
// 		       ), *AbilitySpecHandle.ToString());
// 	}
//
// 	// Process all abilities that had their input released this frame.
// 	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
// 	{
// 		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
// 		{
// 			if (AbilitySpec->Ability)
// 			{
// 				AbilitySpec->InputPressed = false; // Mark input as released on the spec
//
// 				if (AbilitySpec->IsActive())
// 				{
// 					AbilitySpecInputReleased(*AbilitySpec);
// 					UE_LOG(LogTemp, Log,
// 					       TEXT(
// 						       "UMSAbilitySystemComponent::ProcessAbilityInput: Ability released: '%s'"
// 					       ), *GetNameSafe(AbilitySpec->Ability));
// 				}
// 			}
// 		}
// 	}
//
// 	// Clear the cached ability handles for the next tick
// 	InputPressedSpecHandles.Reset();
// 	InputReleasedSpecHandles.Reset();
// 	// InputHeldTags and InputHeldSpecHandles are NOT reset as they track the continuous state
// }

void UMSAbilitySystemComponent::GiveAbilitiesFromInputConfig(const UMSInputConfig* InputConfig, AActor* InOwnerActor)
{
	if (!InputConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("UMSAbilitySystemComponent::GiveAbilitiesFromInputConfig: InputConfig is null!"));
		return;
	}

	for (const FMSInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid() && Action.AbilityClass)
		// Ensure AbilityClass is set in your FMSInputAction struct
		{
			// When granting abilities, you must associate the InputTag with the ability.
			// Lyra uses DynamicAbilityTags for this.
			FGameplayAbilitySpec NewAbilitySpec(Action.AbilityClass, 1, static_cast<int32>(Action.InputID),
			                                    InOwnerActor);
			NewAbilitySpec.GetDynamicSpecSourceTags().AddTag(Action.InputTag);
			// This is how the ability knows what input tag triggered it!

			// Set the ability's activation policy based on how you want it to behave with input.
			// This requires a custom UMSGameplayAbility class. For now, you can just grant it.
			// UMSGameplayAbility* MSAbilityCDO = Cast<UMSGameplayAbility>(Action.AbilityClass->GetDefaultObject());
			// if (MSAbilityCDO)
			// {
			//     NewAbilitySpec.ActivationPolicy = MSAbilityCDO->GetActivationPolicy(); // If you implement this
			// }

			FGameplayAbilitySpecHandle AbilityHandle = GiveAbility(NewAbilitySpec);
			// You might want to store AbilityHandle if you need to revoke it later.
		}
	}
	UE_LOG(LogTemp, Log,
	       TEXT(
		       "UMSAbilitySystemComponent::GiveAbilitiesFromInputConfig: %d abilities granted from InputConfig."
	       ), GetActivatableAbilities().Num());
}

// These helper functions from Lyra are for dynamically binding/unbinding abilities,
// often for UI abilities or abilities granted/removed on the fly.
// For now, you might not need to implement them fully until you get to advanced scenarios.
// void UMSAbilitySystemComponent::ProcessAbilityInput_BindTagToAbility(FGameplayAbilitySpec& Spec) { /* ... */ }
// void UMSAbilitySystemComponent::ProcessAbilityInput_ClearBinding(FGameplayAbilitySpec& Spec) { /* ... */ }

// void UMSAbilitySystemComponent::Deactivate()
// {
// 	Super::Deactivate();
// 	UE_LOG(LogTemp, Warning, TEXT("UMSAbilitySystemComponent DEACTIVATED! Owner: %s, Avatar: %s"),
// 	       *GetNameSafe(GetOwner()), *GetNameSafe(GetAvatarActor()));
//
// 	UE_LOG(LogTemp, Warning, TEXT("Stack Trace in the next line:"));
// 	// Print call stack to log
// 	FDebug::DumpStackTraceToLog(ELogVerbosity::Type::Error);
// }

// void UMSAbilitySystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
// {
// 	Super::EndPlay(EndPlayReason);
// 	UE_LOG(LogTemp, Warning, TEXT("UMSAbilitySystemComponent EndPlay: %s, Reason: %d"),
// 	       *GetNameSafe(this), (int32)EndPlayReason);
// }
