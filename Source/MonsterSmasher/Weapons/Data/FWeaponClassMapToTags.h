#pragma once


#include "CoreMinimal.h"
#include "Engine/DataTable.h" // Required if you plan to use this struct in a data table
#include "GameplayTagContainer.h"
#include "FWeaponClassMapToTags.generated.h"

/**
 * This struct maps Gameplay Tags to Weapon Classes.
 */
USTRUCT(BlueprintType)
struct FWeaponClassMapToTags : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AWeaponBase> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (Categories = "Weapon"))
	FGameplayTag GameplayTag;
};
