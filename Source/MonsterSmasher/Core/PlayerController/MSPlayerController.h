// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "Input/MSInputConfig.h"
#include "MSPlayerController.generated.h"

class UW_MSGameHUD;
class UInputMappingContext;

/**
 * Manage HUD and Inputs
 */
UCLASS()
class MONSTERSMASHER_API AMSPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	UW_MSGameHUD* GetGameHUD() const;

protected:
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;
	
	// --------------- Input Mapping ---------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> PlayerInputMappingContext;

	// --------------- HUD ---------------
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UW_MSGameHUD> GameHUDClass;

private:
	UPROPERTY()
	TObjectPtr<UW_MSGameHUD> GameHUD;
};
