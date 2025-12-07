// Fill out your copyright notice in the Description page of Project Settings.



#include "W_MSGameHUD.h"

#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"
#include "Vitals/W_Vitals.h"
#include "AbilitiesBar/W_AbilitiesBar.h"


void UW_MSGameHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Optional: Additional logic here if needed
}

void UW_MSGameHUD::InitializeHUDWithGAS(UMSAbilitySystemComponent* InASC, UMSAttributeSet* InAttributes) const
{
	if (!InASC || !InAttributes)
	{
		return;
	}

	// Initialize HUD and Sub widgets
	Vitals->InitializeWithGAS(InASC, InAttributes);
	AbilitiesBar->InitializeWithGAS(InASC);
}

UW_Vitals* UW_MSGameHUD::GetVitalsWidget() const
{
	return Vitals;
}

UW_AbilitiesBar* UW_MSGameHUD::GetAbilitiesBarWidget() const
{
	return AbilitiesBar;
}
