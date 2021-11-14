// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SKData.h"
#include "Abilities/GameplayAbility.h"
#include "SKGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class SUNKENCOLONY_API USKGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	public:
	USKGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="GAS")
	SKAbilityInputID AbilityInputID {SKAbilityInputID::None};
	
};
