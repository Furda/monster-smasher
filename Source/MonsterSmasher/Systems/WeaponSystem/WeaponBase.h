// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

struct FWeaponMeshConfig;
class USkeletalMesh;
class UWeaponDataAsset;
class UBoxComponent;

/**
 * Base weapon actor - handles both static and skeletal mesh weapons
 * Configured entirely through UWeaponDataAsset
 */
UCLASS()
class MONSTERSMASHER_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();
	
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void BeginPlay() override;
	
public:
	
	/**
	 * Initialize weapon with data asset configuration
	 * Sets up mesh, collision, and stores reference to data
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Initialize(UWeaponDataAsset* InWeaponData);

	/**
	 * Attach weapon to a ParentMesh
	 * @param ParentMesh - The skeletal mesh to attach to (usually character mesh)
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AttachToCharacter(USceneComponent* ParentMesh);
	
	/**
	 * Enable/disable weapon collision for hit detection
	 * Called during attack animation notifies
	 */
	// UFUNCTION(BlueprintCallable, Category = "Weapon")
	// void SetWeaponCollisionEnabled(bool bEnabled);
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	UWeaponDataAsset* GetWeaponData() const { return WeaponData; }

	/**
	* Reference to the data asset that configured this weapon
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data", ReplicatedUsing=OnRep_WeaponData)
	TObjectPtr<UWeaponDataAsset> WeaponData;
	
protected:
	
	/** Neutral root for the weapon actor */
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon|Components")
	TObjectPtr<USceneComponent> WeaponRoot;
	
	/** Tracks the active mesh components spawned for this weapon */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Components", ReplicatedUsing=OnRep_WeaponParts)
	TArray<TObjectPtr<UStaticMeshComponent>> ActiveWeaponParts;
	
	/** Set up the static mesh of a weapon part */
	void InitializeWeaponStaticMesh(const FWeaponMeshConfig& WeaponPart);
	
	/** Disable collision of a weapon part mesh */
	void DisableWeaponPartColission(UStaticMeshComponent* WeaponPartMesh);
	
	/**
	* Collision box for hit detection during attacks
	* Positioned to cover the weapon's damage-dealing area
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UBoxComponent> HitCollision;
	
	/**
	 * Actors hit during current attack (prevents multi-hitting)
	 * Cleared at start of each attack
	 */
	UPROPERTY()
	TArray<TObjectPtr<AActor>> HitActors;

	// UFUNCTION()
	// void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	// 					UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	// 					bool bFromSweep, const FHitResult& SweepResult);
	
	
	// ===================================
	// Replication functions
	// ===================================

	UFUNCTION()
	void OnRep_WeaponData(UWeaponDataAsset* OldWeaponData);
	
	UFUNCTION()
	void OnRep_WeaponParts(TArray<UStaticMeshComponent*> OldWeaponParts);
};
