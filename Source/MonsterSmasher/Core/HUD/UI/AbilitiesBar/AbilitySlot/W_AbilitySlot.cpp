// Fill out your copyright notice in the Description page of Project Settings.



#include "W_AbilitySlot.h"

#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"
#include "Systems/GAS/Abilities/MSGameplayAbility.h"
#include "Abilities/GameplayAbilityTypes.h"
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
	if (CachedGameplayAbility)
	{
		CachedGameplayAbility->OnCommittedCooldown.RemoveDynamic(this, &UW_AbilitySlot::OnCooldownStarted);
	}

	Super::NativeDestruct();
}

void UW_AbilitySlot::InitializeWithGAS(UMSAbilitySystemComponent* InASC, UMSGameplayAbility* InGameplayAbility)
{
	if (!InASC || !InGameplayAbility)
	{
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("UW_AbilitySlot::InitializeWithGAS: for ability: %s"), *InGameplayAbility->GetName());

	// cache references
	CachedASC = InASC;
	CachedGameplayAbility = InGameplayAbility;

	// Set Ability slot properties
	AbilityText->SetText(FText::FromName(CachedGameplayAbility->GetFName()));

	if (CachedGameplayAbility->AbilityIcon)
	{
		AbilityIcon->SetBrushFromTexture(CachedGameplayAbility->AbilityIcon);
		AbilityText->SetVisibility(ESlateVisibility::Hidden);
	}


	// Bind abilities cooldown events
	CachedGameplayAbility->OnCommittedCooldown.AddDynamic(this, &UW_AbilitySlot::OnCooldownStarted);
}

void UW_AbilitySlot::OnCooldownStarted(const FGameplayTagContainer& CooldownTags, float CooldownDuration)
{
	UE_LOG(LogTemp, Warning,
	       TEXT("UW_AbilitySlot::OnCooldownStarted: UI Cooldown started! Duration: %.2f seconds for Cooldown tag: %s"),
	       CooldownDuration, *CooldownTags.ToString());

	CooldownOverlay->SetVisibility(ESlateVisibility::Visible);


	if (!CachedGameplayAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Cached ability."));
		return;
	}

	FGameplayAbilityActorInfo ActorInfoCopy = CachedGameplayAbility->GetActorInfo();

	UMSAbilitySystemComponent* ASC = ActorInfoCopy.AbilitySystemComponent.IsValid()
		                                 ? Cast<UMSAbilitySystemComponent>(ActorInfoCopy.AbilitySystemComponent.Get())
		                                 : nullptr;
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("No AbilitySystemComponent."));
		return;
	}

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
	float TimeRemaining = GetCooldownTimeRemaining();

	UE_LOG(LogTemp, Warning, TEXT("Timer Fired! Time remaining: %f"), TimeRemaining);

	if (TimeRemaining <= 0.f)
	{
		OnCooldownEnded();
		return;
	}

	SetIsEnabled(false);
	if (CooldownOverlay && CooldownText)
	{
		FText CooldownTimeText = FText::FromString(FString::Printf(TEXT("%.1fs"), TimeRemaining));
		CooldownText->SetText(CooldownTimeText);
		CooldownOverlay->SetVisibility(TimeRemaining > 0.f ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

float UW_AbilitySlot::GetCooldownTimeRemaining()
{
	if (!CachedASC || !CachedGameplayAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ASC or ability."));
		return 0.f;
	}

	FGameplayAbilitySpec* Spec = CachedASC->FindAbilitySpecFromClass(CachedGameplayAbility->GetClass());
	if (!Spec || !Spec->Ability)
	{
		return 0.f;
	}

	float TimeRemaining = 0.f, Duration = 0.f;
	Spec->Ability->GetCooldownTimeRemainingAndDuration(Spec->Handle, CachedASC->AbilityActorInfo.Get(), TimeRemaining,
	                                                   Duration);
	return TimeRemaining;
}

void UW_AbilitySlot::OnCooldownEnded()
{
	SetIsEnabled(true);

	// This should hide the cooldown overlay bar and its children
	CooldownOverlay->SetVisibility(ESlateVisibility::Hidden);

	// Clear timer
	// CooldownTimerHandle.Invalidate();
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
}
