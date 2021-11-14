// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "SKAttributeSet.generated.h"

/**
 * 
 */

// Uses macros from AttributeSet.h, these help avoiding lots of boilerplate code, example usages below
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SUNKENCOLONY_API USKAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	USKAttributeSet();

	// health attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category="Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USKAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category="Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USKAttributeSet, MaxHealth);

	// Movement speed attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoveSpeed, Category="Attributes")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(USKAttributeSet, MoveSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMoveSpeed, Category="Attributes")
	FGameplayAttributeData MaxMoveSpeed;
	ATTRIBUTE_ACCESSORS(USKAttributeSet, MaxMoveSpeed);

	// Lane switch speed multiplier Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LaneSwitchSpeed, Category="Attributes")
	FGameplayAttributeData LaneSwitchSpeed;
	ATTRIBUTE_ACCESSORS(USKAttributeSet, LaneSwitchSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxLaneSwitchSpeed, Category="Attributes")
	FGameplayAttributeData MaxLaneSwitchSpeed;
	ATTRIBUTE_ACCESSORS(USKAttributeSet, MaxLaneSwitchSpeed);

	// Replication init
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called before EVERY attribute being modified e.g. by GameplayEffect
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
private:
	
	// Adjusts "current" attribute relatively to "max" attribute, called when "max" attribute is being changed.
	// in other words it preserves "current" attribute percentage value relative to "max" attribute
	void AdjustAttributeForMaxChange(
		const FGameplayAttributeData& AffectedAttribute,
		const FGameplayAttributeData& MaxAttribute,
		float NewMaxValue,
		const FGameplayAttribute& EffectedAttributeProperty) const;

	// OnRep_ functions needed for replication only????
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_MaxMoveSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_LaneSwitchSpeed(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_MaxLaneSwitchSpeed(const FGameplayAttributeData& OldValue);
};
