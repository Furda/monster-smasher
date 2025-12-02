// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_UnequipWeapon.h"
#include "GameplayTagContainer.h"
#include "GameplayTags/MyNativeGameplayTags.h"
#include "Weapons/WeaponManagerComponent.h"


UGA_UnequipWeapon::UGA_UnequipWeapon()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	ActivationPolicy = EMSAbilityActivationPolicy::OnInputTriggered;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bReplicateInputDirectly = true;
	SetAssetTags(FGameplayTagContainer(TAG_Ability_Utility_UnequipWeapon));
}

void UGA_UnequipWeapon::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	// Always call the Super implementation first
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Commit cost/cooldown
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}

	// Get Weapon manager component from avatar actor 
	if (!ActorInfo->AvatarActor.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("UGA_UnequipWeapon::ActivateAbility: ActorInfo->AvatarActor is invalid!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UWeaponManagerComponent* WeaponManager = ActorInfo->AvatarActor->FindComponentByClass<UWeaponManagerComponent>();

	if (!WeaponManager)
	{
		UE_LOG(LogTemp, Error, TEXT("UGA_UnequipWeapon::ActivateAbility: WeaponManagerComponent is invalid!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	// Unequip weapon by the following weapon tag
	WeaponManager->UnequipWeapon(false);

	// End ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_UnequipWeapon::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
