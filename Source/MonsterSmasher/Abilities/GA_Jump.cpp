// Fill out your copyright notice in the Description page of Project Settings.



#include "GA_Jump.h"

#include "GameFramework/Character.h"
#include "GameplayTags/MyNativeGameplayTags.h"

UGA_Jump::UGA_Jump()
{
	ActivationPolicy = EMSAbilityActivationPolicy::OnInputTriggered;
	SetAssetTags(FGameplayTagContainer(TAG_InputTag_Jump));
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo,
                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbilityCost(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Error, TEXT("UGA_Jump: CommitAbilityCost failed!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	// Call the native jump function on the character
	if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		Character->Jump();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UGA_Jump::ActivateAbility: AvatarActor is not a character!"));
	}
	
	if (!CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true))
	{
		UE_LOG(LogTemp, Error, TEXT("UGA_Jump: CommitAbilityCost failed!"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	// End the ability immediately after jumping
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
