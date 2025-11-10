// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "Input/MSInputConfig.h"
#include "MSPlayerController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class MONSTERSMASHER_API AMSPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	UUserWidget* GetGameHUD() const;

protected:
	virtual void BeginPlay() override;

	// --------------- Input Mapping ---------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> PlayerInputMappingContext;

	// --------------- HUD ---------------
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UUserWidget> GameHUDClass;

private:
	UPROPERTY()
	TObjectPtr<UUserWidget> GameHUD;
};
