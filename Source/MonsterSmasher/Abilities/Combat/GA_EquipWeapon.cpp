// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_EquipWeapon.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayTagContainer.h"
#include "GameplayTags/MyNativeGameplayTags.h"
#include "Weapons/WeaponManagerComponent.h"

UGA_EquipWeapon::UGA_EquipWeapon()
{
	// An ability that needs to respond to client-side input release
	// MUST be set to LocalPredicted.
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// Set Activation policy
	ActivationPolicy = EMSAbilityActivationPolicy::OnInputTriggered;

	// We want this ability to be INSTANCED so we can track the EffectHandle
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Ensure input replication for predicted abilities
	bReplicateInputDirectly = true;

	// Set the tag for this specific ability
	SetAssetTags(FGameplayTagContainer(TAG_Ability_Utility_EquipWeapon));

	// Set the Gameplay Event to activate this Ability
	// FAbilityTriggerData TriggerEventData;
	// TriggerEventData.TriggerTag = FGameplayTags::Event_Weapon_Equip;
	// TriggerEventData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	// AbilityTriggers.Add(TriggerEventData);
}

void UGA_EquipWeapon::ActivateAbility(
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

	// If triggered from event we can access TriggerEventData to get the data of the event
	if (!TriggerEventData)
	{
		UE_LOG(LogTemp, Error, TEXT("UGA_EquipWeapon::ActivateAbility: TriggerEventData is invalid!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Get a reference to the internal array of the target tags.
	const TArray<FGameplayTag>& TargetTagsArray = TriggerEventData->TargetTags.GetGameplayTagArray();

	// Check if the array contains any tags.
	FGameplayTag FollowingWeaponTag = (TargetTagsArray.Num() > 0)
		                                  ? TargetTagsArray[0]
		                                  : FGameplayTag::EmptyTag;

	// Get Weapon manager component from avatar actor 
	if (!ActorInfo->AvatarActor.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("UGA_EquipWeapon::ActivateAbility: ActorInfo->AvatarActor is invalid!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UWeaponManagerComponent* WeaponManager = ActorInfo->AvatarActor->FindComponentByClass<UWeaponManagerComponent>();

	if (!WeaponManager)
	{
		UE_LOG(LogTemp, Error, TEXT("UGA_EquipWeapon::ActivateAbility: WeaponManagerComponent is invalid!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Equip weapon by the weapon tag
	// WeaponManager->EquipWeaponByTag(WeaponTag);

	// Equip weapon by the following weapon tag
	WeaponManager->EquipWeaponByFollowingTag(FollowingWeaponTag);

	// End ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}


void UGA_EquipWeapon::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
