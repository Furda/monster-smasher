// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_CombatBase.h"
#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"



bool UGA_CombatBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	// 1. Run standard checks first
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// 2. Check the Ability System Component for incapacitating tags
	if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
	{
		// If the character is stunned, they cannot activate any combat ability
		if (StunnedTag.IsValid() && ASC->HasMatchingGameplayTag(StunnedTag))
		{
			return false;
		} 

		// If the ability is an attack ability (not the block ability itself), check if the character is blocking.
		// The block ability should be able to activate when block tag is not present.
		if (BlockingTag.IsValid() && ASC->HasMatchingGameplayTag(BlockingTag))
		{
			// If we are blocking, we can still activate the Block ability to refresh its duration, 
			// or if we are using a hold-to-block mechanic, we allow re-activation if needed.
			// For now, let's just disallow it if the BlockingTag is present, assuming this GA is an attack.
			// (We will manage the Block GA separately below)
			return false;
		}
	}
    
	return true;
}
