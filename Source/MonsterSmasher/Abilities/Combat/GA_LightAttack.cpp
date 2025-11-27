// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_LightAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AnimNotifies/AN_HitBoxStart.h"
#include "Characters/Base/MSCharacterBase.h"
#include "Systems/GAS/Tasks/WaitAnimNotifyHitBox.h"
#include "GameplayTags/MyNativeGameplayTags.h"
// #include "Components/CombatComponent.h" // Combo logic


UGA_LightAttack::UGA_LightAttack()
{
	// Set the tag for this specific ability
	SetAssetTags(
		FGameplayTagContainer(TAG_InputTag_Attack_Light));
	// FGameplayTag::RequestGameplayTag(FName("Ability.Attack.Light"))

	// Ensure we can only activate locally predicted on the client
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Attack should be blocked by the Block tag
	ActivationBlockedTags.AddTag(TAG_State_Blocking);
}

void UGA_LightAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Commit the ability to ensure it can be used
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		// If CommitAbility fails (e.g., cooldown not ready), or no montage is set, end immediately.
		UE_LOG(LogTemp, Warning, TEXT("UGA_LightAttack: CommitAbility failed!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (!AttackMontage)
	{
		// If no montage is set, end immediately (or log an error)
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		UE_LOG(LogTemp, Error, TEXT("UGA_LightAttack: No AttackMontage set! Ending ability immediately."));
	}

	// Create the task to play the montage and wait for it to finish
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None, // Task Instance Name (optional)
		AttackMontage,
		1.0f, // Rate
		"Melee01", // Start Section
		true // Should interrupt when ability is canceled
	);

	// Bind the completion and interruption events
	MontageTask->OnCompleted.AddDynamic(this, &UGA_LightAttack::OnMontageFinished);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_LightAttack::OnMontageFinished);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_LightAttack::OnMontageFinished);
	MontageTask->OnBlendOut.AddDynamic(this, &UGA_LightAttack::OnMontageFinished);

	// Start the task
	MontageTask->ReadyForActivation();

	// Wait for the Hit Box Notify to trigger
	// NOTE: The NotifyName MUST match the name of the Anim Notify you place in the montage.
	UWaitAnimNotifyHitBox* HitBoxTask = UWaitAnimNotifyHitBox::WaitForHitBoxNotify(
		this,
		FName("HitBoxStartWait"),
		UAN_HitBoxStart::StaticClass(),
		AttackMontage
	);

	// Bind the notify fire event to the damage function (OnHitBoxOpen)
	HitBoxTask->OnHitBoxWindowOpen.AddDynamic(this, &UGA_LightAttack::OnHitBoxOpen);
	HitBoxTask->OnAbilityCancelled.AddDynamic(this, &UGA_LightAttack::OnHitBoxTaskCancelled);
	HitBoxTask->ReadyForActivation();


	// *** FUTURE COMBO LOGIC GOES HERE: ***
	// We will call a function on the Combat Component to advance the combo state here
}

void UGA_LightAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                 bool bWasCancelled)
{
	// Stop any active montage if the ability is ending early
	if (IsActive())
	{
		AMSCharacterBase* Character = Cast<AMSCharacterBase>(ActorInfo->AvatarActor.Get());
		if (Character)
		{
			UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
			if (AnimInstance && AnimInstance->Montage_IsPlaying(AttackMontage))
			{
				AnimInstance->Montage_Stop(0.25f, AttackMontage);
			}
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_LightAttack::OnMontageFinished()
{
	// Clean up and end the ability when the montage is done.
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_LightAttack::OnHitBoxOpen()
{
	// --- SECTION 5.1 TEST MILESTONE: HIT-BOX LOGIC STARTS HERE ---
	// For now, we will simply log that the hit window is open.
	UE_LOG(LogTemp, Warning, TEXT("!!! LIGHT ATTACK HIT WINDOW OPENED !!!"));

	// In Section 5.2, we will replace this log with the actual trace/overlap check
	// and apply the damage Gameplay Effect if an enemy is hit.
}

void UGA_LightAttack::OnHitBoxTaskCancelled()
{
	UE_LOG(LogTemp, Warning, TEXT("Light Attack HitBox Task was cancelled (e.g., ability cancelled mid-swing)."));
}
