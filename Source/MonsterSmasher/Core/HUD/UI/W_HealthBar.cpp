// Fill out your copyright notice in the Description page of Project Settings.


#include "W_HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Characters/Player/MSPlayerCharacter.h"
#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"
#include "Systems/GAS/Attributes/MSAttributeSet.h"

void UW_HealthBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UE_LOG(LogTemp, Warning, TEXT("UW_HealthBar::NativeOnInitialized Initializing Health Bar"));

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UW_HealthBar::NativeOnInitialized No PlayerController found!"));
		return;
	}

	AMSPlayerCharacter* MSPlayerCharacter = Cast<AMSPlayerCharacter>(PC->GetPawn());
	if (!MSPlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("UW_HealthBar::NativeOnInitialized No AMSPlayerCharacter found!"));
		return;
	}

	UAbilitySystemComponent* ASC = MSPlayerCharacter->GetAbilitySystemComponent();
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("UW_HealthBar::NativeOnInitialized No AbilitySystemComponent found on PlayerCharacter!"));
		return;
	}

	// Bind to health attribute delegate
	ASC->GetGameplayAttributeValueChangeDelegate(UMSAttributeSet::GetHealthAttribute())
	   .AddUObject(this, &UW_HealthBar::OnHealthChanged);

	// Set initial value
	UE_LOG(LogTemp, Warning, TEXT("UW_HealthBar::NativeOnInitialized Initializing Health Bar Values with attributes"));
	const UMSAttributeSet* AttrSet = MSPlayerCharacter->GetAttributeSet();
	UpdateHealthBar(AttrSet->GetHealth(), AttrSet->GetMaxHealth());
}

void UW_HealthBar::InitializeWithAbilitySystem(UMSAbilitySystemComponent* InASC, UMSAttributeSet* InAttributes)
{
	if (!InASC || !InAttributes)
	{
		return;
	}

	// Bind delegate
	InASC->GetGameplayAttributeValueChangeDelegate(InAttributes->GetHealthAttribute())
	     .AddUObject(this, &UW_HealthBar::OnHealthChanged);

	UE_LOG(LogTemp, Warning,
	       TEXT("UW_HealthBar::InitializeWithAbilitySystem Initializing Health Bar Values with attributes"));
	// Force initial sync
	UpdateHealthBar(InAttributes->GetHealth(), InAttributes->GetMaxHealth());
}

void UW_HealthBar::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	AMSPlayerCharacter* PlayerCharacter = Cast<AMSPlayerCharacter>(PC->GetPawn());
	if (!PlayerCharacter)
	{
		return;
	}

	const UMSAttributeSet* AttrSet = PlayerCharacter->GetAttributeSet();
	UpdateHealthBar(Data.NewValue, AttrSet->GetMaxHealth());
}

void UW_HealthBar::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	const float Percent = MaxHealth > 0.f ? CurrentHealth / MaxHealth : 0.f;

	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
		UE_LOG(LogTemp, Warning, TEXT("HealthBar: Set Percent to %f"), Percent);
	}

	if (HealthText)
	{
		HealthText->SetText(FText::AsNumber(FMath::RoundToInt(CurrentHealth)));
		UE_LOG(LogTemp, Warning, TEXT("HealthBar: Set Text to %d"), FMath::RoundToInt(CurrentHealth));
	}
}
