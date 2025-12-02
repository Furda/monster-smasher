// Fill out your copyright notice in the Description page of Project Settings.


#include "MSPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Core/PlayerState/MSPlayerState.h"
#include "Systems/GAS/Attributes/MSAttributeSet.h"
#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemComponent.h"
#include "Core/PlayerController/MSPlayerController.h"
#include "Weapons/WeaponManagerComponent.h"


// =======================
// Set up and overrides
// =======================

AMSPlayerCharacter::AMSPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// ----- Initialize Character collision and movement -----

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Configure character movement
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// ---- Initialize Camera Components ----

	// Create a camera boom (pulls in towards the player if there's a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The AbilitySystemComponent and AttributeSet will be owned by PlayerState as per our plan.

	// ----- Initialize the Weapon Manager Component -----

	// NOTE: This might need to change when we create the Combat Component since the weapon manager component could live inside the combat component
	WeaponManager = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("WeaponManager"));
}

void AMSPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMSPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// You can add any per-tick logic here if needed.
	// For GAS, most logic is handled by the AbilitySystemComponent.
}

void AMSPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AMSPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}


// =======================
// Basic movement set up
// =======================

void AMSPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMSPlayerCharacter::Move);
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMSPlayerCharacter::Look);
		}

		// Bind ability inputs using the new Input Config
		if (AMSPlayerController* PC = Cast<AMSPlayerController>(GetController()))
		{
			PC->BindAbilityInput(EnhancedInputComponent, AbilityInputConfig);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! This template requires the Enhanced Input plugin to be enabled!"
		       ), *GetNameSafe(this));
	}
}

void AMSPlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMSPlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


// =======================
// GAS Set up and overrides
// =======================

UAbilitySystemComponent* AMSPlayerCharacter::GetAbilitySystemComponent() const
{
	// Retrieve the PlayerState from the character, cast it to your custom player state,
	// and then get its AbilitySystemComponent.
	AMSPlayerState* PS = GetPlayerState<AMSPlayerState>();
	return (PS ? PS->GetAbilitySystemComponent() : nullptr);
}

UMSAbilitySystemComponent* AMSPlayerCharacter::GetMSAbilitySystemComponent() const
{
	AMSPlayerState* PS = GetPlayerState<AMSPlayerState>();
	return (PS ? PS->GetMSAbilitySystemComponent() : nullptr);
}

UMSAttributeSet* AMSPlayerCharacter::GetAttributeSet() const
{
	AMSPlayerState* PS = GetPlayerState<AMSPlayerState>();
	return (PS ? PS->GetAttributeSet() : nullptr);
}

void AMSPlayerCharacter::InitAbilitySystemAndAttributes()
{
	// Call the base class to initialize the Ability System and Attributes
	Super::InitAbilitySystemAndAttributes();
}

// Grant Starting abilities by Ability Input Config
void AMSPlayerCharacter::GrantStartingAbilities()
{
	// Grant default attributes if the ASC is valid and is in the server
	if (!AbilitySystemComponent || !HasAuthority() || !AbilityInputConfig)
	{
		if (!AbilityInputConfig)
		{
			UE_LOG(LogTemp, Error, TEXT("AMSPlayerCharacter::GrantStartingAbilities - AbilityInputConfig is null!"));
		}
		return;
	}

	// Grant abilities using the ability input config and save the handles to the granted abilities array
	GrantedAbilities = AbilitySystemComponent->GiveAbilitiesFromInputConfig(AbilityInputConfig, this);

	SendAbilitiesChangedEvent();
}
