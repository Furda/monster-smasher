// Fill out your copyright notice in the Description page of Project Settings.


#include "MSGameplayAbility.h"


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
	// For now, it just calls the base. Blueprints will override this.
	UE_LOG(LogTemp, Log, TEXT("UMSGameplayAbility ActivateAbility: %s"), *GetName());
}

void UMSGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                    bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	UE_LOG(LogTemp, Log, TEXT("UMSGameplayAbility Ended: %s (Was Cancelled: %s)"), *GetName(),
	       bWasCancelled ? TEXT("True") : TEXT("False"));

	// Final cleanup logic here.
}
