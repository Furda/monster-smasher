// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/GAS/Abilities/MSGameplayAbility.h"
#include "GA_EquipWeapon.generated.h"

/**
 *  
 */
UCLASS()
class MONSTERSMASHER_API UGA_EquipWeapon : public UMSGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_EquipWeapon();

	// Called when the ability is activated.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	// Called when the ability ends (e.g., when the input is released).
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
};
