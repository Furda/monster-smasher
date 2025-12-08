// Fill out your copyright notice in the Description page of Project Settings.



#include "W_AbilitySlot.h"

#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"
#include "Systems/GAS/Abilities/MSGameplayAbility.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "GameplayTagContainer.h"



void UW_AbilitySlot::NativeConstruct()
{
	Super::NativeConstruct();

	// Init UI widgets
	if (CooldownOverlay && CooldownText)
	{
		CooldownText->SetText(FText::FromString(TEXT("0s")));
		CooldownOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UW_AbilitySlot::NativeDestruct()
{
	if (CachedASC)
	{
		CachedASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
	}

	Super::NativeDestruct();
}

void UW_AbilitySlot::InitializeWithGAS(UMSAbilitySystemComponent* InASC, FGameplayAbilitySpecHandle InAbilitySpecHandle)
{
	if (!InASC || !InAbilitySpecHandle.IsValid())
	{
		return;
	}

	// cache references
	CachedASC = InASC;
	CachedAbility = Cast<UMSGameplayAbility>(CachedASC->FindAbilitySpecFromHandle(InAbilitySpecHandle)->Ability);
	
	// Set Ability slot properties
	if (CachedAbility->AbilityIcon)
	{
		AbilityIcon->SetBrushFromTexture(CachedAbility->AbilityIcon);
	}
	
	// Get The tag to watch from the ability's cooldown GameplayEffect
	if (!CachedAbility->GetCooldownGameplayEffect())
	{
		UE_LOG(LogTemp, Log, TEXT("UW_AbilitySlot::InitializeWithGAS: No cooldown GameplayEffect found for the ability: %s."), *InAbilitySpecHandle.ToString());
		return;
	}
	
	FGameplayTagContainer CooldownGETags = CachedAbility->GetCooldownGameplayEffect()->GetAssetTags();
	if (!CooldownGETags.IsValid() || CooldownGETags.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("UW_AbilitySlot::InitializeWithGAS: No cooldown tags found for the ability: %s."), *InAbilitySpecHandle.ToString());
		return;
	}
	CooldownTagToWatch = CooldownGETags.First();
	
	// Bind delegate to listen for new GameplayEffects applied to the owner
	CachedASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &UW_AbilitySlot::OnGameplayEffectAdded);
}

void UW_AbilitySlot::OnGameplayEffectAdded(UAbilitySystemComponent* Target, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle ActiveHandle)
{
	// Ensure this effect is applied to our ASC
	if (Target != CachedASC)
	{
		return;
	}
	 
	// Check if it is the correct gameplay effect by tag
	FGameplayTagContainer CooldownGETags;
	Spec.GetAllAssetTags(CooldownGETags);
	if (!CooldownGETags.IsValid() || CooldownGETags.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("UW_AbilitySlot::OnGameplayEffectAdded: No cooldown tags found for the ability: %s."), *CachedAbilitySpecHandle.ToString());
		return; 
	}
	
	
	if (CooldownGETags.HasTagExact(CooldownTagToWatch))
	{
		// Handle cooldown started
		OnCooldownStarted();
	}
}

void UW_AbilitySlot::OnCooldownStarted()
{
	if (!CachedAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Cached ability."));
		return;
	}
	
	// Show Cooldown UI
	CooldownOverlay->SetVisibility(ESlateVisibility::Visible);
	// SetIsEnabled(false);

	// Update cooldown without delay
	UpdateCooldown();

	// Start Cooldown timer
	const UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(
		CooldownTimerHandle,
		this,
		&UW_AbilitySlot::UpdateCooldown,
		CooldownUpdateRate,
		true
	);
}

void UW_AbilitySlot::UpdateCooldown()
{
	float TimeRemaining = 0.f, Duration = 0.f;
	CachedAbility->GetCooldownTimeRemainingAndDuration(CachedAbilitySpecHandle, CachedASC->AbilityActorInfo.Get(), TimeRemaining,Duration);
	
	if (TimeRemaining <= 0.f)
	{
		OnCooldownEnded();
		return;
	}
	
	if (CooldownOverlay && CooldownText)
	{
		// FText CooldownTimeText = FText::FromString(FString::Printf(TEXT("%.1fs"), TimeRemaining));
		FText CooldownTimeText = FText::AsNumber(FMath::RoundToInt(TimeRemaining));
		CooldownText->SetText(CooldownTimeText);
	}
}

void UW_AbilitySlot::OnCooldownEnded()
{
	// SetIsEnabled(true);

	// This should hide the cooldown overlay bar and its children
	CooldownOverlay->SetVisibility(ESlateVisibility::Hidden);

	// Clear timer
	// CooldownTimerHandle.Invalidate();
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
}
