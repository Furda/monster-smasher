// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/GAS/Abilities/MSGameplayAbility.h"
#include "GA_Jump.generated.h"

/**
 * UGA_Jump is a gameplay ability class inheriting from UMSGameplayAbility.
 *
 * This class represents a jump ability. It encapsulates
 * functionality and configuration specific to the jump mechanic, leveraging
 * Unreal Engine's Gameplay Ability System.
 *
 * Inherits from UMSGameplayAbility, which provides customizable ability activation
 * policies and an overrideable framework for activation, cancellation, and ending events.
 */
UCLASS()
class MONSTERSMASHER_API UGA_Jump : public UMSGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Jump();

protected:
	// Override the core ActivateAbility function
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	// Optional: Override OnAbilityCancelled and EndAbility if you need specific cleanup for Jump.
	// virtual void OnAbilityCancelled(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	// virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
