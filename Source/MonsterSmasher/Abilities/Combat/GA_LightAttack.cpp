// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_LightAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
// #include "Components/CombatComponent.h" // Combo logic



UGA_LightAttack::UGA_LightAttack()
{
    // Set the tag for this specific ability
    SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("InputTag.Attack.Light"))));
    // FGameplayTag::RequestGameplayTag(FName("Ability.Attack.Light"))
    
    // Ensure we can only activate locally predicted on the client
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    
    // Attack should be blocked by the Block tag
    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Blocking")));
}

void UGA_LightAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    
    // Commit the ability to ensure it can be used
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        UE_LOG(LogTemp, Warning, TEXT("UGA_LightAttack: CommitAbility failed!"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }
    
    if (AttackMontage)
    {
        // Create the task to play the montage and wait for it to finish
        UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
            this,
            NAME_None, // Task Instance Name (optional)
            AttackMontage,
            1.0f,      // Rate
            "Melee01", // Start Section
            true       // Should interrupt when ability is canceled
        );

        // Bind the completion and interruption events
        MontageTask->OnCompleted.AddDynamic(this, &UGA_LightAttack::OnMontageFinished);
        MontageTask->OnInterrupted.AddDynamic(this, &UGA_LightAttack::OnMontageFinished);
        MontageTask->OnCancelled.AddDynamic(this, &UGA_LightAttack::OnMontageFinished);
        MontageTask->OnBlendOut.AddDynamic(this, &UGA_LightAttack::OnMontageFinished);

        // Start the task
        MontageTask->ReadyForActivation();
    }
    else
    {
        // If no montage is set, end immediately (or log an error)
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        UE_LOG(LogTemp, Error, TEXT("UGA_LightAttack: No AttackMontage set! Ending ability immediately."));
    }
    
    // *** FUTURE COMBO LOGIC GOES HERE: ***
    // We will call a function on the Combat Component to advance the combo state here
}

void UGA_LightAttack::OnMontageFinished()
{
    // Clean up and end the ability when the montage is done.
    bool bReplicateEndAbility = true;
    bool bWasCancelled = false;
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
