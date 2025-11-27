#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // Required if you plan to use this struct in a data table
#include "FMovementWeaponConfig.generated.h"


/**
 * MovementProperties: Holds all the movement-related properties for weapons.
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
