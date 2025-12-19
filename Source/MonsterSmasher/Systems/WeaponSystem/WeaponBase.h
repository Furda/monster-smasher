// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

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
	 * Attach weapon to character at specified socket
	 * @param ParentMesh - The skeletal mesh to attach to (usually character mesh)
	 * @param SocketName - Socket name on the parent mesh (e.g., "RightHandSocket")
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AttachToCharacter(USceneComponent* ParentMesh, FName SocketName);
	
	/**
	 * Enable/disable weapon collision for hit detection
	 * Called during attack animation notifies
	 */
	// UFUNCTION(BlueprintCallable, Category = "Weapon")
	// void SetWeaponCollisionEnabled(bool bEnabled);
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	UWeaponDataAsset* GetWeaponData() const { return WeaponData; }
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	/**
	* Reference to the data asset that configured this weapon
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data", ReplicatedUsing=OnRep_WeaponData)
	TObjectPtr<UWeaponDataAsset> WeaponData;
	
protected:
	
	/**
	 * Root mesh component - Might want to handle skeletal meshes in the future
	 */
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon|Components")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;
	

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
};
