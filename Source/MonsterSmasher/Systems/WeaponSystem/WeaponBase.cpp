// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"

#include "WeaponManagerComponent.h"
#include "Systems/WeaponSystem/Data/WeaponDataAsset.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	// Create root mesh component
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh.Get());

	// Disable collision on mesh itself (we use HitCollision box instead)
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	// Replicate mesh component
	WeaponMesh->SetIsReplicated(true);
	
	
	// Create hit detection collision box
	HitCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollision"));
	HitCollision->SetupAttachment(WeaponMesh);
    
	// TODO: Check if we need logic for hit detection since we are going to use trace for the hit detection
	// Configure collision - only detect pawns during attacks
	HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Disabled by default
	HitCollision->SetCollisionObjectType(ECC_WorldDynamic);
	HitCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    
	// Set default box size (will be overridden by weapon data)
	HitCollision->SetBoxExtent(FVector(50.f, 10.f, 10.f));
    
	// Bind overlap event
	// HitCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnWeaponOverlap);
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponBase, WeaponData); // Ensure the data itself syncs!
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
    
	// Ensure collision starts disabled
	// SetWeaponCollisionEnabled(false);
}

void AWeaponBase::Initialize(UWeaponDataAsset* InWeaponData)
{
	if (!InWeaponData)
	{
		UE_LOG(LogTemp, Error, TEXT("AWeaponBase::Initialize - WeaponData is null!"));
		return;
	}

	WeaponData = InWeaponData;

	// Configure mesh based on what's provided in the data asset
	if (InWeaponData->WeaponStaticMesh)
	{
		// Use static mesh (for rigid weapons like swords, axes)
		WeaponMesh->SetStaticMesh(InWeaponData->WeaponStaticMesh);
        
		UE_LOG(LogTemp, Log, TEXT("AWeaponBase::Initialize - Set static mesh: %s"), 
			*InWeaponData->WeaponStaticMesh->GetName());
	}
	// Handle skeletal mesh case if needed
	// else if (InWeaponData->WeaponStaticMesh) 
	// {
	// 	// Use skeletal mesh (for animated weapons like whips, chains)
	// 	WeaponMesh->SetSkeletalMesh(InWeaponData->WeaponSkeletalMesh);
 //        
	// 	UE_LOG(LogTemp, Log, TEXT("AWeaponBase::Initialize - Set skeletal mesh: %s"), 
	// 		*InWeaponData->WeaponSkeletalMesh->GetName());
	// }
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AWeaponBase::Initialize - No mesh provided in WeaponData!"));
	}

	// Configure hit collision box
	if (InWeaponData->HitCollisionExtent != FVector::ZeroVector)
	{
		HitCollision->SetBoxExtent(InWeaponData->HitCollisionExtent);
	}
	
	if (InWeaponData->HitCollisionOffset != FVector::ZeroVector)
	{
		HitCollision->SetRelativeLocation(InWeaponData->HitCollisionOffset);
	}

	// Apply any material overrides from weapon data
	if (InWeaponData->WeaponMaterial)
	{
		WeaponMesh->SetMaterial(0, InWeaponData->WeaponMaterial);
	}
}

void AWeaponBase::AttachToCharacter(USceneComponent* ParentMesh, FName SocketName)
{
	if (!ParentMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("AWeaponBase::AttachToCharacter - ParentMesh is null!"));
		return;
	}

	// Verify socket exists on parent mesh
	if (USkeletalMeshComponent* ParentSkeletal = Cast<USkeletalMeshComponent>(ParentMesh))
	{
		if (!ParentSkeletal->DoesSocketExist(SocketName))
		{
			UE_LOG(LogTemp, Error, 
				TEXT("AWeaponBase::AttachToCharacter - Socket '%s' does not exist on parent mesh!"), 
				*SocketName.ToString());
			return;
		}
	}

	// Attach weapon to character
	FAttachmentTransformRules AttachRules(
		EAttachmentRule::SnapToTarget,  // Location
		EAttachmentRule::SnapToTarget,  // Rotation
		EAttachmentRule::KeepWorld,     // Scale
		true                             // Weld simulated bodies
	);

	AttachToComponent(ParentMesh, AttachRules, SocketName);

	UE_LOG(LogTemp, Log, 
		TEXT("AWeaponBase::AttachToCharacter - Attached to socket '%s'"), 
		*SocketName.ToString());
}

// TODO: Check if we need this since Hit detection will be done through a trace
// void AWeaponBase::SetWeaponCollisionEnabled(bool bEnabled)
// {
// 	if (!HitCollision)
// 		return;
//
// 	if (bEnabled)
// 	{
// 		// Clear hit actors list for new attack
// 		HitActors.Empty();
//         
// 		// Enable collision
// 		HitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
//         
// 		UE_LOG(LogTemp, Verbose, TEXT("AWeaponBase::SetWeaponCollisionEnabled - Collision ENABLED"));
// 	}
// 	else
// 	{
// 		// Disable collision
// 		HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//         
// 		UE_LOG(LogTemp, Verbose, TEXT("AWeaponBase::SetWeaponCollisionEnabled - Collision DISABLED"));
// 	}
// }

// TODO: Check if we need this since Hit detection will be done through a trace
// void AWeaponBase::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
// 								   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
// 								   bool bFromSweep, const FHitResult& SweepResult)
// {
// 	if (!OtherActor || OtherActor == GetOwner())
// 		return;
//
// 	// Prevent hitting the same actor multiple times in one swing
// 	if (HitActors.Contains(OtherActor))
// 		return;
//
// 	HitActors.Add(OtherActor);
//
// 	// Send gameplay event to owner's ability system
// 	// The active combat ability will handle damage application
// 	if (AActor* Owner = GetOwner())
// 	{
// 		FGameplayEventData EventData;
// 		EventData.Instigator = Owner;
// 		EventData.Target = OtherActor;
// 		EventData.EventTag = FGameplayTag::RequestGameplayTag("Combat.Event.WeaponHit");
//
// 		// Send to owner's ASC (combat ability will be listening for this)
// 		if (UAbilitySystemComponent* ASC = Owner->FindComponentByClass<UAbilitySystemComponent>())
// 		{
// 			ASC->HandleGameplayEvent(EventData.EventTag, &EventData);
// 		}
//
// 		UE_LOG(LogTemp, Log, 
// 			TEXT("AWeaponBase::OnWeaponOverlap - Hit actor: %s"), 
// 			*OtherActor->GetName());
// 	}
// }


// ===================================
// Replication functions
// ===================================

void AWeaponBase::OnRep_WeaponData(UWeaponDataAsset* OldWeaponData)
{
	// Notify the owning character that data is ready
	
	if (UWeaponManagerComponent* WeaponManager = GetOwner()->FindComponentByClass<UWeaponManagerComponent>())
	{
		WeaponManager->OnWeaponDataReady(WeaponData);
	}
}
