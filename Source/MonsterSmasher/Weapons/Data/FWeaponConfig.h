#pragma once

#include "CoreMinimal.h"
#include "FMovementWeaponConfig.h"
#include "Engine/DataTable.h" // Required if you plan to use this struct in a data table
#include "GameplayTagContainer.h"
#include "FWeaponConfig.generated.h"


// Delegate for any specific events the weapon needs (optional)
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponDataChange);

/**
 * FWeaponData: Holds all the essential, non-runtime statistics and assets for a weapon.
 */
USTRUCT(BlueprintType)
struct FWeaponConfig
{
	GENERATED_BODY()

	// --- IDENTIFICATION & TAGS ---

	/** Unique identifier for this weapon type (e.g., "Weapon.Rifle.Assault") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	FGameplayTag WeaponTag;

	/** Display name of the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	FText Name;

	// --- MESH & ASSETS ---

	/** Static Mesh asset for this weapon (used if the weapon doesn't need complex animation) */
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	// TObjectPtr<class UStaticMesh> StaticMesh;

	/** Skeletal Mesh asset for this weapon (used if the weapon is animated) */
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	// TObjectPtr<class USkeletalMesh> SkeletalMesh;

	/** Default material to apply to the weapon mesh */
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	// TObjectPtr<class UMaterialInterface> DefaultMaterial;

	// --- COMBAT STATS ---

	/** The base amount of damage this weapon inflicts per hit/shot. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float BaseDamage;

	/** The base range of the weapon (for hit detection/tracing). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Range;

	/** The type of damage this weapon deals (e.g., "Damage.Type.Kinetic", "Damage.Type.Energy"). */
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	// FGameplayTag DamageTypeTag;
	//
	// /** The stamina cost (if any) to perform a primary attack. */
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	// float StaminaCost;

	// --- ANIMATION ---

	/** Name of the socket to attach the weapon when is equipped*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FName EquippedSocketName;

	/** Name of the socket to attach the weapon when is not equipped*/
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	// FName UnequippedSocketName;

	/** The Anim Montage to play when using the primary attack. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSubclassOf<class UAnimInstance> AnimClass;

	// --- Abilities ---

	/** The Gameplay Abilities to grant when this weapon is equipped */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> AbilitiesToGrant;

	// --- Movement Configs ---

	/** Movement config of the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FMovementWeaponConfig MovementWeaponConfig;
};
