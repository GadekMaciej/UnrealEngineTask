#pragma once

// Enum for different abilities. Add new Abilities below Punch
UENUM(BlueprintType)
enum class SKAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Punch, 
	// new ability
};