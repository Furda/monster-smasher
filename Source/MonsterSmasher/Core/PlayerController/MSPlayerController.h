// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "Input/MSInputConfig.h"
#include "MSPlayerController.generated.h"

class UW_MSGameplayHUD;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class MONSTERSMASHER_API AMSPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	UW_MSGameplayHUD* GetGameHUD() const;

protected:
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;
	
	// --------------- Input Mapping ---------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> PlayerInputMappingContext;

	// --------------- HUD ---------------
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UUserWidget> GameplayHUDClass;

private:
	UPROPERTY()
	TObjectPtr<UW_MSGameplayHUD> GameplayHUD;
};
