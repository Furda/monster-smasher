// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Block.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


UGA_Block::UGA_Block()
{
    // An ability that needs to respond to client-side input release
    // MUST be set to LocalPredicted.
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
    
    // Set Activation policy
    ActivationPolicy = EMSAbilityActivationPolicy::WhileInputActive;
    
    // We want this ability to be INSTANCED so we can track the EffectHandle
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    
    // Ensure input replication for predicted abilities
    bReplicateInputDirectly = true;
    
    // Set the tag for this specific ability
    SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("InputTag.Block"))));
    // FGameplayTag::RequestGameplayTag(FName("Ability.Defense.Block"))
    
    
    // Blocking is generally not blocked by other things, as it's a defensive response, 
    // but the GA_CombatBase still checks for State.Stunned.
    
    // Note: If you want to prevent Dodge while blocking, add:
    // BlockedAbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Defense.Dodge")));
}

void UGA_Block::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    
    // Commit the ability to ensure it can be used
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        UE_LOG(LogTemp, Warning, TEXT("UGA_Block: CommitAbility failed!"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }
    
    if (BlockingStanceGE && ActorInfo->AbilitySystemComponent.IsValid())
    {
        UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
        
        // Apply the Blocking Stance Gameplay Effect
        FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
        FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(BlockingStanceGE, GetAbilityLevel(), EffectContext);
        
        if (SpecHandle.IsValid())
        {
            // Apply the GE and store the handle
            BlockingEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
        
        // Set up the WaitInputRelease task ONLY on the locally controlled instance.
        if (ActorInfo->IsLocallyControlled())
        {
            UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
            if (WaitInputReleaseTask)
            {
                // Bind the delegate to our function
                WaitInputReleaseTask->OnRelease.AddDynamic(this, &UGA_Block::OnBlockInputReleased);
            
                // Activate the task
                WaitInputReleaseTask->ReadyForActivation();
            }
        }
    }
    
    // If we failed to apply the GE, or if we want to immediately end on a single press, 
    // we would call EndAbility() here. Since Block is generally a HOLD ability, we wait.
}

void UGA_Block::OnBlockInputReleased(float TimeWaited)
{
    // This log confirms the task is now firing correctly.
    UE_LOG(LogTemp, Log, TEXT("UGA_Block: Input released and ending ability. Time held: %.2f"), TimeWaited);
    // End the ability when input is released
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Block::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    // Prevent double-ending if possible
    if (!IsActive())
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("UGA_Block: EndAbility called. Was Cancelled: %s"), bWasCancelled ? TEXT("True") : TEXT("False"));
    
    if (ActorInfo->AbilitySystemComponent.IsValid() && BlockingEffectHandle.IsValid())
    {
        // 1. Remove the Blocking Stance Gameplay Effect
        ActorInfo->AbilitySystemComponent.Get()->RemoveActiveGameplayEffect(BlockingEffectHandle);
        
        // 2. Clear the handle
        BlockingEffectHandle.Invalidate();
    }
    
    // Call the super implementation last
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    // Clear stored task pointer (optional)
    WaitInputReleaseTaskInstance = nullptr;
}