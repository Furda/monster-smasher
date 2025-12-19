// Fill out your copyright notice in the Description page of Project Settings.



#include "GA_LightAttack.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/Base/MSCharacterBase.h"
#include "AnimNotifies/AN_HitBoxStart.h"
#include "Systems/GAS/Tasks/WaitAnimNotifyHitBox.h"
#include "Systems/CombatSystem/CombatSystemComponent.h"
#include "GameplayTags/MyNativeGameplayTags.h"


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
	if (!CommitAbilityCost(Handle, ActorInfo, ActivationInfo))
	{
		// If CommitAbility fails (e.g., cooldown not ready), or no montage is set, end immediately.
		UE_LOG(LogTemp, Error, TEXT("UGA_LightAttack: CommitAbilityCost failed!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	// Get Combat System Component
	UCombatSystemComponent* CombatComp = GetAvatarActorFromActorInfo()
		->FindComponentByClass<UCombatSystemComponent>();
	
	if (!CombatComp)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}
	
	// TODO: Check if we need failures tags and if we need to send a gameplay event on failure
	// Check if the combat component can activate the ability 
	FGameplayTagContainer FailureTags;	
	if (!CombatComp->CanActivateAbility(AbilityDataTag, FailureTags))
	{
		// Optionally send gameplay event with failure reason for UI feedback
		FGameplayEventData EventData;
		EventData.EventTag = FGameplayTag::RequestGameplayTag("Combat.Event.AbilityFailed");
		EventData.InstigatorTags = FailureTags;
        
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetAvatarActorFromActorInfo(), 
			EventData.EventTag, 
			EventData
		);
        
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	// Register this attack with combo system
	CombatComp->RegisterComboAttack(AbilityDataTag);

	// Get the appropriate animation for current combo step
	AttackMontage = CombatComp->GetCurrentComboMontage(AbilityDataTag);
	if (!AttackMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Create the task to play the montage and wait for it to finish
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AttackMontage
	);

	// Bind the completion and interruption events
	MontageTask->OnCompleted.AddDynamic(this, &UGA_LightAttack::OnMontageFinished);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_LightAttack::OnMontageFinished);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_LightAttack::OnMontageFinished);
	MontageTask->OnBlendOut.AddDynamic(this, &UGA_LightAttack::OnMontageFinished);
	MontageTask->ReadyForActivation();

	// --------------------- Hit trace: Gameplay Task -------------------------
	
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
	
	// --------------------- Hit trace: Gameplay Event -------------------------
	
	// Listen for hit events from anim notifies
	// UAbilityTask_WaitGameplayEvent* HitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	// 	this,
	// 	FGameplayTag::RequestGameplayTag("Combat.Event.WeaponHit")
	// );
	// HitEventTask->EventReceived.AddDynamic(this, &UGA_LightAttack::OnWeaponHitEvent);
	// HitEventTask->ReadyForActivation();


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
		if (AMSCharacterBase* Character = Cast<AMSCharacterBase>(ActorInfo->AvatarActor.Get()))
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
	if (!CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true))
	{
		UE_LOG(LogTemp, Error, TEXT("UGA_LightAttack: CommitAbilityCooldown failed!"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	// Reset combo on interrupt (e.g., got hit)
	UCombatSystemComponent* CombatComp = GetAvatarActorFromActorInfo()
		->FindComponentByClass<UCombatSystemComponent>();
	if (CombatComp)
	{
		CombatComp->ResetCombo();
	}
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
