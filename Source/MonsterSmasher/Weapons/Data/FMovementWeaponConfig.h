#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FMovementWeaponConfig.generated.h"


/**
 *  Holds all the movement-related properties for weapons.
 */
USTRUCT(BlueprintType)
struct FMovementWeaponConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MaxWalkSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool OrientRotationToMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool UseControllerDesiredRotation;
};
