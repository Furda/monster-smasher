// Fill out your copyright notice in the Description page of Project Settings.


#include "W_MSHUD.h"
#include "W_HealthBar.h"


void UW_MSHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Optional: Additional logic here if needed
}

UW_HealthBar* UW_MSHUD::GetHealthBarWidget() const
{
	return HealthBar;
}
