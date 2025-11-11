// Fill out your copyright notice in the Description page of Project Settings.


#include "W_HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Characters/Player/MSPlayerCharacter.h"
#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"
#include "Systems/GAS/Attributes/MSAttributeSet.h"

void UW_HealthBar::NativeConstruct()
{
	Super::NativeConstruct();
	// Initial setup if needed
}

void UW_HealthBar::InitializeWithAbilitySystem(UMSAbilitySystemComponent* InASC, UMSAttributeSet* InAttributes)
{
	UE_LOG(LogTemp, Warning, TEXT("UW_HealthBar::InitializeWithAbilitySystem"));
	if (!InASC || !InAttributes)
	{
		return;
	}

	// cache references
	CachedASC = InASC;
	CachedAttributes = InAttributes;

	// 1. Remove the previous binding if it exists
	if (InASC->GetGameplayAttributeValueChangeDelegate(InAttributes->GetHealthAttribute()).IsBoundToObject(this))
	{
		InASC->GetGameplayAttributeValueChangeDelegate(InAttributes->GetHealthAttribute())
			.RemoveAll(this);
	}

	// 2. Bind delegate
	InASC->GetGameplayAttributeValueChangeDelegate(InAttributes->GetHealthAttribute())
	     .AddUObject(this, &UW_HealthBar::OnHealthChanged);

	UE_LOG(LogTemp, Warning,
	       TEXT("UW_HealthBar::InitializeWithAbilitySystem Binding Delegates and setting initial values."));
	
	// 3. Force initial sync
	UpdateHealthBar(InAttributes->GetHealth(), InAttributes->GetMaxHealth());
}

void UW_HealthBar::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (!CachedAttributes)
	{
		return;
	}
	
	UpdateHealthBar(Data.NewValue, CachedAttributes->GetMaxHealth());
}

void UW_HealthBar::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	const float Percent = MaxHealth > 0.f ? CurrentHealth / MaxHealth : 0.f;

	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
		UE_LOG(LogTemp, Warning, TEXT("HealthBar: Set Percent to %f"), Percent);
	}

	if (HealthText)
	{
		HealthText->SetText(FText::AsNumber(FMath::RoundToInt(CurrentHealth)));
		UE_LOG(LogTemp, Warning, TEXT("HealthBar: Set Text to %d"), FMath::RoundToInt(CurrentHealth));
	}
}
