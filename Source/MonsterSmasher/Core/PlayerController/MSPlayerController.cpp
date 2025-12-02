// Fill out your copyright notice in the Description page of Project Settings.


#include "MSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Core/HUD/UI/W_MSGameHUD.h"
#include "Core/PlayerState/MSPlayerState.h"
#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"
#include "EnhancedInputComponent.h"


// =======================
// Set up and overrides
// =======================

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
	if (IsLocalController() && GameHUDClass)
	{
		UW_MSGameHUD* HUDWidget = CreateWidget<UW_MSGameHUD>(this, GameHUDClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
			GameHUD = HUDWidget;

			// --- SERVER / STANDALONE PATH ---
			// The PlayerState is valid for the local controller on the Server/Host early on, 
			// so we use BeginPlay as the server's entry point, bypassing the skipped OnRep.
			TryInitializeHUDWithGAS();
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

	// --- NETWORK CLIENT PATH ---
	// This function is still the correct entry point for network clients 
	// because they must wait for the PlayerState to replicate.
	TryInitializeHUDWithGAS();
}


// =======================
// HUD
// =======================

UW_MSGameHUD* AMSPlayerController::GetGameHUD() const
{
	if (GameHUD)
	{
		return GameHUD;
	}
	UE_LOG(LogTemp, Warning,
	       TEXT("AMSPlayerController: GameHUD is not set! Returning nullptr."
	       ));
	return nullptr;
}

void AMSPlayerController::TryInitializeHUDWithGAS()
{
	if (bIsGameHUDInitialized)
	{
		UE_LOG(LogTemp, Log,
		       TEXT("AMSPlayerController: TryInitializeHUDWithGAS - HUD is already initialized. Skipping."
		       ));
		return;
	}

	if (!GameHUD)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT(
			       "AMSPlayerController: OnRep_PlayerState - HUD is not valid! Cannot initialize HUD with Ability System."
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
			GameHUD->InitializeHUDWithGAS(
				Cast<UMSAbilitySystemComponent>(MSPlayerState->GetAbilitySystemComponent()),
				MSPlayerState->GetAttributeSet()
			);

			// Avoid initializing the HUD more than once
			bIsGameHUDInitialized = true;
		}
	}
}


// =======================
// Player state and ASC properties
// =======================

AMSPlayerState* AMSPlayerController::GetMSPlayerState() const
{
	return CastChecked<AMSPlayerState>(PlayerState);
}

UMSAbilitySystemComponent* AMSPlayerController::GetMSAbilitySystemComponent() const
{
	if (AMSPlayerState* MSPlayerState = GetMSPlayerState())
	{
		return MSPlayerState->GetMSAbilitySystemComponent();
	}
	return nullptr;
}


// =======================
// Ability Binding
// =======================

void AMSPlayerController::BindAbilityInput(UEnhancedInputComponent* EnhancedInputComponent,
                                           UMSInputConfig* AbilityInputConfig)
{
	if (!AbilityInputConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("AMSPlayerController: Missing AbilityInputConfig! Cannot bind ability inputs."));
		return;
	}

	for (const FMSInputAction& Action : AbilityInputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			// Bind the Pressed event (ETriggerEvent::Started)
			EnhancedInputComponent->BindAction(Action.InputAction, ETriggerEvent::Started, this,
			                                   &AMSPlayerController::AbilityInputIDPressed, Action.InputID);

			// Bind the Released event (ETriggerEvent::Completed)
			EnhancedInputComponent->BindAction(Action.InputAction, ETriggerEvent::Completed, this,
			                                   &AMSPlayerController::AbilityInputIDReleased, Action.InputID);
		}
	}
}

// void AMSPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
// {
// 	if (UMSAbilitySystemComponent* MSASC = Cast<UMSAbilitySystemComponent>(GetAbilitySystemComponent()))
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("AMSPlayerController: AbilityInputTagPressed: %s"), *InputTag.ToString());
// 		// Call the custom input function on your ASC
// 		MSASC->AbilityInputTagPressed(InputTag);
// 	}
// }

// void AMSPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
// {
// 	if (UMSAbilitySystemComponent* MSASC = Cast<UMSAbilitySystemComponent>(GetAbilitySystemComponent()))
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("AMSPlayerController: AbilityInputTagReleased: %s"), *InputTag.ToString());
//
// 		// Call the custom input function on your ASC
// 		MSASC->AbilityLocalInputReleased(InputTag);
// 	}
// }

void AMSPlayerController::AbilityInputIDPressed(EAbilityInputID InputID)
{
	if (UMSAbilitySystemComponent* MSASC = Cast<UMSAbilitySystemComponent>(GetMSAbilitySystemComponent()))
	{
		UE_LOG(LogTemp, Log, TEXT("AMSPlayerController: AbilityInputIDPressed: %s"), *UEnum::GetValueAsString(InputID));
		// Call the custom input function on your ASC
		MSASC->AbilityLocalInputPressed(static_cast<int32>(InputID));
	}
}

void AMSPlayerController::AbilityInputIDReleased(EAbilityInputID InputID)
{
	if (UMSAbilitySystemComponent* MSASC = Cast<UMSAbilitySystemComponent>(GetMSAbilitySystemComponent()))
	{
		UE_LOG(LogTemp, Log, TEXT("AMSPlayerController: AbilityInputIDReleased: %s"),
		       *UEnum::GetValueAsString(InputID));

		// Call the custom input function on your ASC
		MSASC->AbilityLocalInputReleased(static_cast<int32>(InputID));
	}
}
