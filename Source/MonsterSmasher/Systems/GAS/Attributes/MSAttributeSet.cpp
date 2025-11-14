// Fill out your copyright notice in the Description page of Project Settings.


#include "MSAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UMSAttributeSet::UMSAttributeSet()
{
}

void UMSAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Clamp Health
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	// Clamp Stamina
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
}

void UMSAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Get the Target Ability System Component
	UAbilitySystemComponent* TargetASC = &Data.Target;
	if (!TargetASC)
	{
		return;
	}

	// All attributes should be modify by the Meta attributes, not directly by the actual attributes.
	// This is a common pattern to ensure that the Meta attributes are used for calculations.

	// Get the Modified Attribute
	FGameplayAttribute ModifiedAttribute = Data.EvaluatedData.Attribute;
	
	// The Data.OldValue here refers to the pre-GE attribute value.

	// Handle Meta Health changes 
	if (ModifiedAttribute == GetMetaHealthAttribute())
	{
		const float NewHealth = FMath::Clamp(GetHealth() + GetMetaHealth(), 0.0f, GetMaxHealth());
		SetHealth(NewHealth);
		// UE_LOG(LogTemp, Log, TEXT("Health changed. Meta Health applied: %f, Current Health: %f"), GetMetaHealth(),
		//        GetHealth());

		// Reset Meta Health to zero after applying
		SetMetaHealth(0.0f);
	}
	// Handle Meta Stamina changes
	else if (ModifiedAttribute == GetMetaStaminaAttribute())
	{
		const float NewStamina = FMath::Clamp(GetStamina() + GetMetaStamina(), 0.0f, GetMaxStamina());
		SetStamina(NewStamina);
		// UE_LOG(LogTemp, Log, TEXT("Stamina changed. Meta Stamina applied: %f, Current Stamina: %f"), GetMetaStamina(),
		//        GetStamina());

		// Reset Meta Stamina to zero after applying
		SetMetaStamina(0.0f);
	}
	// ... handle other attributes if needed
}

// ---------- Replication Overrides ----------

void UMSAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate Health attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UMSAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMSAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	// Replicate Stamina attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UMSAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMSAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);

	// Repeat for other attributes...
}

// ---------- Health Replication ----------

void UMSAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMSAttributeSet, Health, OldHealth);
}

void UMSAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMSAttributeSet, MaxHealth, OldMaxHealth);
}

// ---------- Stamina Replication ----------

void UMSAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMSAttributeSet, Stamina, OldStamina);
}

void UMSAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMSAttributeSet, MaxStamina, OldMaxStamina);
}
