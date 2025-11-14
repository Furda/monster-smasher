// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Sprint.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"

UGA_Sprint::UGA_Sprint()
{
	ActivationPolicy = EMSAbilityActivationPolicy::WhileInputActive;
	SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("InputTag.Sprint"))));
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}


void UGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Commit the ability to ensure it can be used
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGA_Sprint: CommitAbility failed!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Character)
	{
		CharacterMovement = Character->GetCharacterMovement();
		if (CharacterMovement)
		{
			OriginalWalkSpeed = CharacterMovement->MaxWalkSpeed; // Store original speed
			CharacterMovement->MaxWalkSpeed *= SprintSpeedRate; // Set sprint speed

			// Create a WaitInputRelease task to end the sprint when the input is released
			UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
			if (WaitInputReleaseTask)
			{
				// Bind the callback function
				WaitInputReleaseTask->OnRelease.AddDynamic(this, &UGA_Sprint::OnSprintInputReleased);
				// Activate the task
				WaitInputReleaseTask->ReadyForActivation();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UGA_Sprint: CharacterMovementComponent not found."));
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true); // End if unable to sprint
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UGA_Sprint: Failed to cast AvatarActor to Character."));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true); // End if unable to sprint
	}
}

void UGA_Sprint::OnSprintInputReleased(float TimeWaited)
{
	// End the ability when input is released
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                            const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                            bool bWasCancelled)
{
	// Only call Super::EndAbility once
	if (!IsActive())
	{
		return;
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// Ensure speed is reset when ability ends naturally
	// Ensure speed is reset when ability ends (either naturally or cancelled)
	if (CharacterMovement && CharacterMovement->MaxWalkSpeed != OriginalWalkSpeed)
	// Only reset if it's not already original
	{
		CharacterMovement->MaxWalkSpeed = OriginalWalkSpeed;
		// UE_LOG(LogTemp, Log, TEXT("UGA_Sprint: Sprint Ended. Speed reset to %f."), OriginalWalkSpeed);
	}
}
