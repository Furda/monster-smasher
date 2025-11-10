// Fill out your copyright notice in the Description page of Project Settings.


#include "MSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Characters/Player/MSPlayerCharacter.h"

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

	// Add HUD widget to the viewport
	if (IsLocalPlayerController() && GameHUDClass)
	{
		UUserWidget* HUDWidget = CreateWidget<UUserWidget>(this, GameHUDClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
}

UUserWidget* AMSPlayerController::GetGameHUD() const
{
	return GameHUD;
}
