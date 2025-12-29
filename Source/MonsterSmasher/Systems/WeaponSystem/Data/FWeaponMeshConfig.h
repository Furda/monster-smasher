#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FWeaponMeshConfig.generated.h"

USTRUCT(BlueprintType)
struct FWeaponMeshConfig
{
	GENERATED_BODY()

	/** Skeletal mesh for animated weapons (whips, chains, articulated weapons) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Skeletal Mesh (Optional)"))
	TObjectPtr<USkeletalMesh> WeaponSkeletalMesh;
	
	/** Static mesh for rigid weapons (swords, axes, hammers) - more performant */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Static Mesh (Optional)"))
	TObjectPtr<UStaticMesh> WeaponStaticMesh;

	UPROPERTY(EditDefaultsOnly)
	FName SocketName; // e.g., "LeftHandSocket" or "RightHandSocket"
};