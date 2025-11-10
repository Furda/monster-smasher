// Fill out your copyright notice in the Description page of Project Settings.


#include "MSGameMode.h"
#include "Core/PlayerController/MSPlayerController.h"
#include "Core/PlayerState/MSPlayerState.h"
#include "Characters/Base/MSCharacterBase.h"
#include "Core/HUD/MSHUD.h"

AMSGameMode::AMSGameMode()
{
	PlayerControllerClass = AMSPlayerController::StaticClass();
	PlayerStateClass = AMSPlayerState::StaticClass();
	DefaultPawnClass = AMSCharacterBase::StaticClass();
	HUDClass = AMSHUD::StaticClass();
}