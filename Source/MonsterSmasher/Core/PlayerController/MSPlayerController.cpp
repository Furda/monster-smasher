// Fill out your copyright notice in the Description page of Project Settings.


#include "MSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Core/HUD/UI/W_MSGameHUD.h"
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
	if (IsLocalPlayerController() && GameHUDClass)
	{
		UW_MSGameHUD* HUDWidget = CreateWidget<UW_MSGameHUD>(this, GameHUDClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
			GameHUD = HUDWidget;
		}
		else
		{
			UE_LOG(LogTemp, Warning,
			       TEXT("AMSPlayerController: GameplayHUDClass is not valid! HUD will not be created."
			       ));
		}
	}
}

void AMSPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Initialized HUD with GAS
	UW_MSGameHUD* HUD = GetGameHUD();

	if (!HUD)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("AMSPlayerController: OnRep_PlayerState - HUD is not valid! Cannot initialize HUD with Ability System."
		       ));
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
			HUD->InitializeHUDWithGAS(
				Cast<UMSAbilitySystemComponent>(MSPlayerState->GetAbilitySystemComponent()),
				MSPlayerState->GetAttributeSet()
			);
		}
	}
}

UW_MSGameHUD* AMSPlayerController::GetGameHUD() const
{
	if (GameHUD)
	{
		return GameHUD;
	}
	else
	{
		UE_LOG(LogTemp, Warning,
				   TEXT("AMSPlayerController: GameHUD is not set! Returning nullptr."
				   ));
		return nullptr;
	}
}
