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
#include "Input/MSInputConfig.h"
#include "Input/AbilityInputID.h"


AMSPlayerCharacter::AMSPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

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

// --------------- GAS Set up ---------------

UAbilitySystemComponent* AMSPlayerCharacter::GetAbilitySystemComponent() const
{
	// Retrieve the PlayerState from the character, cast it to your custom player state,
	// and then get its AbilitySystemComponent.
	AMSPlayerState* PS = GetPlayerState<AMSPlayerState>();
	return (PS ? PS->GetAbilitySystemComponent() : nullptr);
}

UMSAttributeSet* AMSPlayerCharacter::GetAttributeSet() const
{
	AMSPlayerState* PS = GetPlayerState<AMSPlayerState>();
	return (PS ? PS->GetAttributeSet() : nullptr);
}


void AMSPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AMSPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
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

	// Use custom Ability granting from the custom ASC
	AbilitySystemComponent->GiveAbilitiesFromInputConfig(AbilityInputConfig, this);

	UE_LOG(LogTemp, Log, TEXT("AMSPlayerCharacter: Granted startup abilities from AbilityInputConfig."));
}


// --------------- Ability Binding ---------------

void AMSPlayerCharacter::BindAbilityInput(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (!AbilityInputConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("AMSPlayerCharacter: Missing AbilityInputConfig! Cannot bind ability inputs."));
		return;
	}

	for (const FMSInputAction& Action : AbilityInputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			// Bind the Pressed event (ETriggerEvent::Started)
			EnhancedInputComponent->BindAction(Action.InputAction, ETriggerEvent::Started, this,
			                                   &AMSPlayerCharacter::AbilityInputIDPressed, Action.InputID);

			// Bind the Released event (ETriggerEvent::Completed)
			EnhancedInputComponent->BindAction(Action.InputAction, ETriggerEvent::Completed, this,
			                                   &AMSPlayerCharacter::AbilityInputIDReleased, Action.InputID);
		}
	}
}

// void AMSPlayerCharacter::AbilityInputTagPressed(FGameplayTag InputTag)
// {
// 	if (UMSAbilitySystemComponent* MSASC = Cast<UMSAbilitySystemComponent>(GetAbilitySystemComponent()))
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("AMSPlayerCharacter: AbilityInputTagPressed: %s"), *InputTag.ToString());
// 		// Call the custom input function on your ASC
// 		MSASC->AbilityInputTagPressed(InputTag);
// 	}
// }

// void AMSPlayerCharacter::AbilityInputTagReleased(FGameplayTag InputTag)
// {
// 	if (UMSAbilitySystemComponent* MSASC = Cast<UMSAbilitySystemComponent>(GetAbilitySystemComponent()))
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("AMSPlayerCharacter: AbilityInputTagReleased: %s"), *InputTag.ToString());
//
// 		// Call the custom input function on your ASC
// 		MSASC->AbilityLocalInputReleased(InputTag);
// 	}
// }

void AMSPlayerCharacter::AbilityInputIDPressed(EAbilityInputID InputID)
{
	if (UMSAbilitySystemComponent* MSASC = Cast<UMSAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		UE_LOG(LogTemp, Log, TEXT("AMSPlayerCharacter: AbilityInputIDPressed: %s"), *UEnum::GetValueAsString(InputID));
		// Call the custom input function on your ASC
		MSASC->AbilityLocalInputPressed(static_cast<int32>(InputID));
	}
}

void AMSPlayerCharacter::AbilityInputIDReleased(EAbilityInputID InputID)
{
	if (UMSAbilitySystemComponent* MSASC = Cast<UMSAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		UE_LOG(LogTemp, Log, TEXT("AMSPlayerCharacter: AbilityInputIDReleased: %s"), *UEnum::GetValueAsString(InputID));
		
		// Call the custom input function on your ASC
		MSASC->AbilityLocalInputReleased(static_cast<int32>(InputID));
	}
}

// -------------- Basic movement Set up ---------------

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

		// if (JumpAction)
		// {
		// 	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMSPlayerCharacter::Jump);
		// 	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
		// 	                                   &AMSPlayerCharacter::StopJumping);
		// }

		// Bind ability inputs using the new Input Config
		BindAbilityInput(EnhancedInputComponent);
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

