// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Base/MSCharacterBase.h"
#include "MSEnemyCharacter.generated.h"

UCLASS()
class MONSTERSMASHER_API AMSEnemyCharacter : public AMSCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMSEnemyCharacter();

protected:
	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual UMSAttributeSet* GetAttributeSet() const override;

	virtual void GrantStartingAbilities() override;

	// Attribute change callbacks (optional to override for player-specific UI updates)
	// virtual void OnHealthChanged(const FOnAttributeChangeData& Data) override;
};
