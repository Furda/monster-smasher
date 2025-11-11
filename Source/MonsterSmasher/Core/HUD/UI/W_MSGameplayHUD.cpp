// Fill out your copyright notice in the Description page of Project Settings.


#include "W_MSGameplayHUD.h"
#include "W_HealthBar.h"


void UW_MSGameplayHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Optional: Additional logic here if needed
}

UW_HealthBar* UW_MSGameplayHUD::GetHealthBarWidget() const
{
	return HealthBar;
}
