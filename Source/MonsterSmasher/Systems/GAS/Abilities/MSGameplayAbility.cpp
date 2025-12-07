// Fill out your copyright notice in the Description page of Project Settings.



#include "MSGameplayAbility.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"

// ======================
// Set up and overrides
// ======================

UMSGameplayAbility::UMSGameplayAbility() : ActivationPolicy(EMSAbilityActivationPolicy::OnInputTriggered)
{
	// Set a default activation policy.
	ActivationPolicy = EMSAbilityActivationPolicy::OnInputTriggered;

	// By default, make abilities instanced per Actor unless specified otherwise
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMSGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Add common activation logic here if needed for all abilities.
	// UE_LOG(LogTemp, Log, TEXT("UMSGameplayAbility ActivateAbility: %s"), *GetName());
}

void UMSGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                    bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// UE_LOG(LogTemp, Log, TEXT("UMSGameplayAbility Ended: %s (Was Cancelled: %s)"), *GetName(), bWasCancelled ? TEXT("True") : TEXT("False"));

	// Final cleanup logic here.
}

UMSAbilitySystemComponent* UMSGameplayAbility::GetMSAbilitySystemComponentFromActorInfo() const
{
	return Cast<UMSAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
}


// ======================
// Activation Properties
// ======================

EMSAbilityActivationPolicy UMSGameplayAbility::GetActivationPolicy() const
{
	return ActivationPolicy;
}


// ======================
// Cooldown Events and overrides
// ======================

bool UMSGameplayAbility::CommitAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                       FGameplayTagContainer* OptionalRelevantTags)
{
	const bool bCommitted = Super::CommitAbility(Handle, ActorInfo, ActivationInfo);
	if (bCommitted)
	{
		BroadcastCooldownCommit();
	}
	return bCommitted;
}

bool UMSGameplayAbility::CommitAbilityCooldown(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo,
                                               const FGameplayAbilityActivationInfo ActivationInfo,
                                               const bool ForceCooldown, FGameplayTagContainer* OptionalRelevantTags)
{
	const bool bCommitted = Super::CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, ForceCooldown);
	if (bCommitted)
	{
		BroadcastCooldownCommit();
	}
	return bCommitted;
}

void UMSGameplayAbility::BroadcastCooldownCommit() const
{
	if (!CooldownGameplayEffectClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("No cooldown GameplayEffect class assigned"));
		return;
	}

	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(
		CooldownGameplayEffectClass, GetAbilityLevel());

	if (SpecHandle.IsValid())
	{
		const FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

		// Get cooldown tag from GameplayEffect's Asset Tags
		FGameplayTagContainer CooldownTags;
		Spec->GetAllAssetTags(CooldownTags);

		// Get duration from Spec's Duration Magnitude
		float CooldownDuration = Spec->GetDuration();

		// Broadcast your event here, passing tags and duration
		OnCommittedCooldown.Broadcast(CooldownTags, CooldownDuration);
	}
}
