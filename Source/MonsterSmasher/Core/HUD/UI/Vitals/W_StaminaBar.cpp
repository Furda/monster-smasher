// Fill out your copyright notice in the Description page of Project Settings.


#include "W_StaminaBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Characters/Player/MSPlayerCharacter.h"
#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"
#include "Systems/GAS/Attributes/MSAttributeSet.h"

void UW_StaminaBar::NativeConstruct()
{
	Super::NativeConstruct();
	// Initial setup if needed
}

void UW_StaminaBar::InitializeWithGAS(UMSAbilitySystemComponent* InASC, UMSAttributeSet* InAttributes)
{
	if (!InASC || !InAttributes)
	{
		return;
	}

	// cache references
	CachedASC = InASC;
	CachedAttributes = InAttributes;

	// 1. Remove the previous binding if it exists
	if (InASC->GetGameplayAttributeValueChangeDelegate(InAttributes->GetStaminaAttribute()).IsBoundToObject(this))
	{
		InASC->GetGameplayAttributeValueChangeDelegate(InAttributes->GetStaminaAttribute())
			.RemoveAll(this);
	}

	// 2. Bind delegate
	InASC->GetGameplayAttributeValueChangeDelegate(InAttributes->GetStaminaAttribute())
	     .AddUObject(this, &UW_StaminaBar::OnStaminaChanged);
	
	// 3. Force initial sync
	UpdateStaminaBar(InAttributes->GetStamina(), InAttributes->GetMaxStamina());
}

void UW_StaminaBar::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	if (!CachedAttributes)
	{
		return;
	}
	
	UpdateStaminaBar(Data.NewValue, CachedAttributes->GetMaxStamina());
}

void UW_StaminaBar::UpdateStaminaBar(float CurrentStamina, float MaxStamina)
{
	const float Percent = MaxStamina > 0.f ? CurrentStamina / MaxStamina : 0.f;

	if (StaminaBar)
	{
		StaminaBar->SetPercent(Percent);
	}

	if (StaminaText)
	{
		StaminaText->SetText(FText::Format(FText::FromStringView(TEXT("{Current} / {Max}")), FFormatNamedArguments{
	{TEXT("Current"), FText::AsNumber(FMath::RoundToInt(CurrentStamina))},
	{TEXT("Max"), FText::AsNumber(FMath::RoundToInt(MaxStamina))}
		}));
	}
}
