// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "MSGameMode.generated.h"

class AMSHUD;
class AMSPlayerController;
class AMSPlayerState;

/**
 * This class defines the game mode for the Monster Smasher game.
 */
UCLASS()
class MONSTERSMASHER_API AMSGameMode : public AModularGameMode
{
	GENERATED_BODY()

public:
	AMSGameMode();
};
