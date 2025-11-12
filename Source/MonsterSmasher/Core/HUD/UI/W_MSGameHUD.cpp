// Fill out your copyright notice in the Description page of Project Settings.


#include "W_MSGameHUD.h"
#include "Vitals/W_Vitals.h"


void UW_MSGameHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Optional: Additional logic here if needed
}

void UW_MSGameHUD::InitializeHUDWithGAS(UMSAbilitySystemComponent* InASC, UMSAttributeSet* InAttributes) const
{
	// Initialize HUD and Sub widgets
	Vitals->InitializeWithGAS(InASC, InAttributes);
}

UW_Vitals* UW_MSGameHUD::GetVitalsWidget() const
{
	return Vitals;
}
