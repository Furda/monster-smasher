// Fill out your copyright notice in the Description page of Project Settings.


#include "MSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Core/HUD/UI/W_MSGameplayHUD.h"
#include "Core/HUD/UI/W_HealthBar.h"
#include "Core/PlayerState/MSPlayerState.h"
#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"


void AMSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context to the local player subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		if (PlayerInputMappingContext)
		{
			Subsystem->AddMappingContext(PlayerInputMappingContext, 0);
		}
		else
		{
			UE_LOG(LogTemp, Warning,
			       TEXT("AMSPlayerController: PlayerInputMappingContext is not set! Input mappings will not be active."
			       ));
		}
	}

	// Add the HUD widget to the viewport
	if (IsLocalPlayerController() && GameplayHUDClass)
	{
		UW_MSGameplayHUD* HUDWidget = CreateWidget<UW_MSGameplayHUD>(this, GameplayHUDClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
			GameplayHUD = HUDWidget;
		}
		else
		{
			UE_LOG(LogTemp, Warning,
			       TEXT("AMSPlayerController: GameplayHUDClass is not valid! HUD will not be created."
			       ));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AMSPlayerController: HUD will not be created."));
	}
}

void AMSPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UW_HealthBar* HealthBarWidget = GetGameHUD()->GetHealthBarWidget();

	if (!HealthBarWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("AMSPlayerController: HealthBarWidget is not set!"));
		return;
	}
	
	// PlayerState is now guaranteed to be valid on the client.
	if (PlayerState)
	{
		// Cast to your custom PlayerState
		AMSPlayerState* MSPlayerState = Cast<AMSPlayerState>(PlayerState);
        
		// Call the widget's initialization function!
		if (MSPlayerState && MSPlayerState->GetAbilitySystemComponent() && MSPlayerState->GetAttributeSet())
		{
			HealthBarWidget->InitializeWithAbilitySystem(
				Cast<UMSAbilitySystemComponent>(MSPlayerState->GetAbilitySystemComponent()),
				MSPlayerState->GetAttributeSet()
			);
		}
	}
}

UW_MSGameplayHUD* AMSPlayerController::GetGameHUD() const
{
	if (GameplayHUD)
	{
		return GameplayHUD;
	}
	else
	{
		UE_LOG(LogTemp, Warning,
				   TEXT("AMSPlayerController: GameHUD is not set! Returning nullptr."
				   ));
		return nullptr;
	}
}
