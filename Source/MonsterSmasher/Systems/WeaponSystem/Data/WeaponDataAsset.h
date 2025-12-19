// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/DataValidation.h"
#include "GameplayTagContainer.h"
#include "FCombatAbilityData.h"
#include "FMovementWeaponConfig.h"
#include "Input/FMSInputAction.h"
#include "Engine/DataAsset.h"
#include "GameplayTags/MyNativeGameplayTags.h"
#include "WeaponDataAsset.generated.h"


class UGameplayAbility;
class UAnimInstance;
class UMaterialInterface;


/**
 *  The complete weapon definition that designers use to create new weapons without code.
 */
UCLASS(BlueprintType)
class MONSTERSMASHER_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	public:
	
    // =====================================================
    // Weapon Identity
    // =====================================================
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Identity")
    FText WeaponName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Identity")
	FGameplayTag WeaponTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Identity",
	   meta = (MultiLine = true))
	FText WeaponDescription;
	
	
	// =====================================================
	// Visual Representation - Choose ONE mesh type
	// =====================================================
	
	/** Skeletal mesh for animated weapons (whips, chains, articulated weapons) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Visual",
		meta = (DisplayName = "Skeletal Mesh (Optional)"))
	TObjectPtr<USkeletalMesh> WeaponSkeletalMesh;
	
	/** Static mesh for rigid weapons (swords, axes, hammers) - more performant */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Visual",
		meta = (DisplayName = "Static Mesh (Optional)"))
	TObjectPtr<UStaticMesh> WeaponStaticMesh;
	
	/** Optional material override for weapon mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Visual")
	TObjectPtr<UMaterialInterface> WeaponMaterial;
	
	// Icon for UI (inventory, HUD)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Visual")
	TObjectPtr<UTexture2D> WeaponIcon;
	
	
	// =====================================================
	// Equipment
	// =====================================================
	
	/** Socket name on character skeleton to attach weapon when equipped */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Equipment")
	FName EquippedSocketName = "RightHandSocket";
	
	/** Socket name on character skeleton when weapon is holstered */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Equipment")
	FName HolsteredSocketName = "BackSocket";
	
	
	// =====================================================
	// Hit Detection
	// =====================================================
	
	/** Box half‑size in local space (X,Y,Z) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Collision")
	FVector HitCollisionExtent = FVector(10.f, 10.f, 10.f);

	/** Local offset from the weapon root (or mesh) to center of hit box */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Collision")
	FVector HitCollisionOffset = FVector::ZeroVector;
	
    // =====================================================
    // Ability Granting
    // =====================================================
	
	/** Abilities granted when weapon is equipped */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Abilities")
    TArray<FMSInputAction> AbilitiesToGrant;

	
    // =====================================================
    // Animation
    // =====================================================
	
	/** Animation blueprint to use when this weapon is equipped */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation")
    TSubclassOf<UAnimInstance> WeaponAnimBP;
	
	
	// =====================================================
	// Movement
	// =====================================================
	
	/** Movement config to use when this weapon is equipped */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Movement")
	FMovementWeaponConfig MovementWeaponConfig;

	
    // =====================================================
    // Combat Data
    // =====================================================
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Combat")
    FCombatAbilityData LightAttackData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Combat")
    FCombatAbilityData HeavyAttackData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Combat")
    FCombatAbilityData BlockData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Combat")
    FCombatAbilityData SpecialAttackData;

	
    // =====================================================
    // Stats
    // =====================================================
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats",
		meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float BaseDamageMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats",
		meta = (ClampMin = "0.1", ClampMax = "3.0"))
	float AttackSpeedMultiplier = 1.0f;

	/** Weight affects character movement speed when equipped */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats",
		meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Weight = 10.0f;

	
	// =====================================================
	// Asset Management
	// =====================================================
	
    // Asset Manager integration
    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId("Weapon", GetFName());
    }
	
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override
    {
    	EDataValidationResult Result = Super::IsDataValid(Context);

	    if (WeaponTag.MatchesTagExact(TAG_Weapon_Type_Unarmed))
	    {
	    	Result = EDataValidationResult::Valid;
	    	return Result;
	    }
    	
    	// Validation: Must have at least one mesh type
    	if (!WeaponSkeletalMesh && !WeaponStaticMesh)
    	{
    		Context.AddError(FText::FromString("Weapon must have either a Skeletal Mesh or Static Mesh assigned!"));
    		Result = EDataValidationResult::Invalid;
    	}

    	// Validation: Warn if both mesh types are set
    	if (WeaponSkeletalMesh && WeaponStaticMesh)
    	{
    		Context.AddWarning(FText::FromString("Both Skeletal and Static Mesh are set. Static Mesh will be used."));
    	}

    	// Validation: Must have at least one ability
    	if (AbilitiesToGrant.Num() == 0)
    	{
    		Context.AddWarning(FText::FromString("Weapon has no abilities granted. Is this intentional?"));
    	}

    	return Result;
    }
#endif
};
