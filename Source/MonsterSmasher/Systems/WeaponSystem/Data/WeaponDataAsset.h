// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "FCombatAbilityData.h"
#include "FMovementWeaponConfig.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"


class UGameplayAbility;
/**
 *  The complete weapon definition that designers use to create new weapons without code.
 */
UCLASS()
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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Identity")
    UTexture2D* WeaponIcon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Equipment")
    FName EquippedSocketName;

	
    // =====================================================
    // Ability Granting
    // =====================================================
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Abilities")
    TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant;

    // =====================================================
    // Animation and Movement Setup
    // =====================================================
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation")
    TSubclassOf<UAnimInstance> WeaponAnimBP;
	
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
    // Optional: Weapon-wide stats
    // =====================================================
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats")
    float BaseDamageMultiplier = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Stats")
    float AttackSpeedMultiplier = 1.0f;

    // Asset Manager integration
    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId("Weapon", GetFName());
    }
};
