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

void UW_HealthBar::InitializeWithGAS(UMSAbilitySystemComponent* InASC, UMSAttributeSet* InAttributes)
{
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
	}

	if (HealthText)
	{
		HealthText->SetText(FText::Format(FText::FromStringView(TEXT("{Current} / {Max}")), FFormatNamedArguments{
	{TEXT("Current"), FText::AsNumber(FMath::RoundToInt(CurrentHealth))},
	{TEXT("Max"), FText::AsNumber(FMath::RoundToInt(MaxHealth))}
		}));
		
	}
}
