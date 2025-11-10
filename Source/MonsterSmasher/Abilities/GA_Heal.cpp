// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Heal.h"
#include "Characters/Base/MSCharacterBase.h"
#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"

UGA_Heal::UGA_Heal()
{
	ActivationPolicy = EMSAbilityActivationPolicy::OnInputTriggered;
	SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.Utility.Heal"))));
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

// UGameplayEffect* UGA_Heal::GetCooldownGameplayEffect() const
// {
// 	return CooldownGameplayEffectClass ? CooldownGameplayEffectClass->GetDefaultObject<UGameplayEffect>() : nullptr;
// }

void UGA_Heal::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo,
                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Commit the ability to ensure it can be used
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGA_Heal: CommitAbility failed!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Apply healing effect to the target character
	AMSCharacterBase* TargetCharacter = Cast<AMSCharacterBase>(ActorInfo->AvatarActor.Get());
	ApplyHealingEffect(TargetCharacter);

	// End the ability after applying the healing effect
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_Heal::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                          const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                          bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UGA_Heal::ApplyHealingEffect(const AMSCharacterBase* TargetCharacter)
{
	UMSAbilitySystemComponent* MSASC = Cast<UMSAbilitySystemComponent>(TargetCharacter->GetAbilitySystemComponent());
	if (TargetCharacter && MSASC && HealingEffect)
	{
		// Create and apply a healing effect
		FGameplayEffectContextHandle EffectContext = MSASC->MakeEffectContext();
		EffectContext.AddSourceObject(MSASC->GetOwner());

		FGameplayEffectSpecHandle CostSpecHandle = MSASC->MakeOutgoingSpec(
			HealingEffect, GetAbilityLevel(), EffectContext);

		if (CostSpecHandle.IsValid())
		{
			CostSpecHandle.Data->SetSetByCallerMagnitude(HealMagnitudeTag, HealingAmount);

			MSASC->ApplyGameplayEffectSpecToSelf(*CostSpecHandle.Data.Get());
			UE_LOG(LogTemp, Log, TEXT("Healing effect applied to %s"), *TargetCharacter->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Character is invalid or does not have an Ability System Component!"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}
