// Fill out your copyright notice in the Description page of Project Settings.


#include "W_Vitals.h"
#include "W_HealthBar.h"
#include "W_StaminaBar.h"


void UW_Vitals::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Optional: Additional logic here if needed
}

void UW_Vitals::InitializeWithGAS(UMSAbilitySystemComponent* InASC, UMSAttributeSet* InAttributes) const
{
	// Initialize HUD and Sub widgets
	HealthBar->InitializeWithGAS(InASC, InAttributes);
	StaminaBar->InitializeWithGAS(InASC, InAttributes);
}

UW_HealthBar* UW_Vitals::GetHealthBarWidget() const
{
	return HealthBar;
}

UW_StaminaBar* UW_Vitals::GetStaminaBarWidget() const
{
	return StaminaBar;
}
