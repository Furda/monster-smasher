// GASPreInclude.h
// Helper to ensure GAS headers are included in correct order
// Include this FIRST in any .cpp that uses GAS

#pragma once

// Core Unreal
#include "CoreMinimal.h"

// GAS headers in dependency order
#include "AttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"