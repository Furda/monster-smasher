// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDManagerComponent.h"
#include "Core/HUD/UI/W_MSGameplayHUD.h"
#include "Core/HUD/UI/W_HealthBar.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "Core/PlayerController/MSPlayerController.h"
#include "Systems/GAS/Attributes/MSAttributeSet.h"
#include "TimerManager.h"

UHUDManagerComponent::UHUDManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHUDManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHUDManagerComponent::InitializeHUD(UMSAbilitySystemComponent* ASC, UMSAttributeSet* Attributes)
{
	CachedASC = ASC;
	CachedAttributes = Attributes;

	SetupHUDInternal();
}

void UHUDManagerComponent::SetupHUDInternal()
{
	if (bIsHUDInitialized)
	{
		return;
	}

	APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUDManager: PlayerController not found, retrying..."));
		GetWorld()->GetTimerManager().SetTimer(RetryInitTimerHandle, this, &UHUDManagerComponent::SetupHUDInternal, 0.1f, false);
		return;
	}

	if (AMSPlayerController* MSController = Cast<AMSPlayerController>(PC))
	{
		GameHUD = Cast<UW_MSGameplayHUD>(MSController->GetGameHUD());
		if (GameHUD && CachedASC && CachedAttributes)
		{
			if (UW_HealthBar* HealthBar = GameHUD->GetHealthBarWidget())
			{
				HealthBar->InitializeWithAbilitySystem(CachedASC, CachedAttributes);
				bIsHUDInitialized = true;
				UE_LOG(LogTemp, Log, TEXT("HUDManager: HealthBar successfully initialized."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("HUDManager: HealthBar not ready yet, retrying..."));
				GetWorld()->GetTimerManager().SetTimer(RetryInitTimerHandle, this, &UHUDManagerComponent::SetupHUDInternal, 0.1f, false);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HUDManager: GameHUD or ASC not ready, retrying..."));
			GetWorld()->GetTimerManager().SetTimer(RetryInitTimerHandle, this, &UHUDManagerComponent::SetupHUDInternal, 0.1f, false);
		}
	}
}


