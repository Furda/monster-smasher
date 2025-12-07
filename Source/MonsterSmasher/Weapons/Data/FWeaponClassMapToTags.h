#pragma once


#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "FWeaponClassMapToTags.generated.h"

class AWeaponBase;

/**
 * This struct maps Gameplay Tags to Weapon Classes.
 */
USTRUCT(BlueprintType)
struct FWeaponClassMapToTags : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeaponBase> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (Categories = "Weapon"))
	FGameplayTag GameplayTag;
};
