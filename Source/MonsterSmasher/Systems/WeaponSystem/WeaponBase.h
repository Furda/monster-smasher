// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UStaticMesh;
class UWeaponDataAsset;

UCLASS()
class MONSTERSMASHER_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// This data asset holds all configuration for the weapon.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UWeaponDataAsset> WeaponData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Use static mesh for simpler weapons and skeletal mesh for complex weapons with moving parts like whip
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;
	// TObjectPtr<USkeletalMeshComponent> WeaponMesh;
};
