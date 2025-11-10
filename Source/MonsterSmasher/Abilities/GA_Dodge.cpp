// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Dodge.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h" // Needed for montage playback
#include "AbilitySystemComponent.h"
#include "Systems/GAS/Attributes/MSAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h" // For GetFloatAttribute


UGA_Dodge::UGA_Dodge()
{
	ActivationPolicy = EMSAbilityActivationPolicy::OnInputTriggered;
	SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("InputTag.Dodge"))));
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

float UGA_Dodge::GetStaminaCost() const
{
	// This function is called to calculate the cost of the ability.
	// This should return the stamina cost for the ability.
	// This ensures consistency with the cost applied in ApplyCost.
	return -FMath::Abs(StaminaCost);
}

// UGameplayEffect* UGA_Dodge::GetCooldownGameplayEffect() const
// {
// 	return CooldownGameplayEffectClass ? CooldownGameplayEffectClass->GetDefaultObject<UGameplayEffect>() : nullptr;
// }

bool UGA_Dodge::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                          OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	// Always call Super::CheckCost first if it has relevant logic, but for Set By Caller,
	// you often replace it entirely or call it after your custom check.
	// In this case, we need to handle the magnitude check ourselves.
	// if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
	// {
	//     return false;
	// }

	UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
	if (!AbilitySystemComponent)
	{
		return false;
	}

	if (!CostGameplayEffectClass || !CostMagnitudeTag.IsValid())
	{
		// No cost defined for this ability, so it "passes" the cost check.
		return true;
	}

	// *** IMPORTANT: Determine the actual cost value here ***
	// This value needs to be consistent with what you'll use in ApplyCost.
	// You should *NOT* try to create a spec and get its magnitude here
	// because the "Set By Caller" magnitude hasn't been set yet.
	// Get it from a consistent source (e.g., a fixed value, a data table, calculation).
	float AbilityStaminaCost = GetStaminaCost();

	bool bSuccessfullyFoundAttribute = false;
	float CurrentStamina = UAbilitySystemBlueprintLibrary::GetFloatAttribute(
		ActorInfo->AbilitySystemComponent->GetOwner(),
		UMSAttributeSet::GetStaminaAttribute(),
		bSuccessfullyFoundAttribute
	);

	if (!bSuccessfullyFoundAttribute)
	{
		UE_LOG(LogTemp, Error, TEXT("%s::CheckCost: Failed to get Stamina attribute for cost check."),
		       *GetNameSafe(this));
		return false; // Can't even check cost if attribute not found
	}

	// Check if the current stamina is greater than or equal to the cost
	if (CurrentStamina >= FMath::Abs(AbilityStaminaCost))
	{
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("%s::CheckCost: Not enough stamina. Current: %f, Needed: %f"),
	       *GetNameSafe(this), CurrentStamina, AbilityStaminaCost);
	// Optionally add relevant tags if you have "NotEnoughX" tags
	// if (OptionalRelevantTags)
	// {
	//    OptionalRelevantTags->AddTag(FGameplayTag::RequestGameplayTag(FName("State.NotEnoughStamina")));
	// }
	return false;
}

void UGA_Dodge::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                          const FGameplayAbilityActivationInfo ActivationInfo) const
{
	// This function is called to apply the cost of the ability.
	// Apply cost effect by caller magnitude tag.
	if (CostGameplayEffectClass && ActorInfo->AbilitySystemComponent.IsValid() && CostMagnitudeTag.IsValid())
	{
		FGameplayEffectContextHandle EffectContext = ActorInfo->AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(ActorInfo->AbilitySystemComponent->GetOwner());

		FGameplayEffectSpecHandle CostSpecHandle = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(
			CostGameplayEffectClass, GetAbilityLevel(), EffectContext);

		if (CostSpecHandle.IsValid())
		{
			// Set the magnitude of the "Set By Caller" tag on the spec
			float ActualCostToApply = GetStaminaCost();
			CostSpecHandle.Data->SetSetByCallerMagnitude(CostMagnitudeTag, ActualCostToApply);

			ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*CostSpecHandle.Data.Get());
			// UE_LOG(LogTemp, Log, TEXT("%s: Applied cost of %f using tag %s."), *GetNameSafe(this), ActualCostToApply,
			//        *CostMagnitudeTag.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UGA_Dodge: CostGameplayEffectClass is null or invalid!"));
	}
}

void UGA_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo,
                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Commit the ability to ensure it can be used
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGA_Dodge: CommitAbility failed!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Check if DodgeMontage is valid
	if (!DodgeMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		UE_LOG(LogTemp, Error, TEXT("UGA_Dodge: DodgeMontage is null!"));
	}

	// Play Dodge Montage: Assuming anim montage has Root Motion enable
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, DodgeMontage, 1.0f, NAME_None, false);

	MontageTask->OnCompleted.AddDynamic(this, &UGA_Dodge::OnDodgeMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_Dodge::OnDodgeMontageCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_Dodge::OnDodgeMontageCompleted);
	MontageTask->ReadyForActivation();
}

void UGA_Dodge::OnDodgeMontageCompleted()
{
	// End the ability when the montage completes
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	// UE_LOG(LogTemp, Log, TEXT("UGA_Dodge: Dodge montage finished."));
}

void UGA_Dodge::OnDodgeFinished()
{
	// UE_LOG(LogTemp, Log, TEXT("UGA_Dodge: Dodge duration finished."));
	// End the ability when the delay completes
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
