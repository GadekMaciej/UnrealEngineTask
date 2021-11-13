// Fill out your copyright notice in the Description page of Project Settings.


#include "SKAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "SKCharacter.h"

USKAttributeSet::USKAttributeSet()
{
}

void USKAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USKAttributeSet, Health);
	DOREPLIFETIME(USKAttributeSet, MaxHealth);
	DOREPLIFETIME(USKAttributeSet, MoveSpeed);
	DOREPLIFETIME(USKAttributeSet, MaxMoveSpeed);
	DOREPLIFETIME(USKAttributeSet, LaneSwitchSpeed);
	DOREPLIFETIME(USKAttributeSet, MaxLaneSwitchSpeed);
}

void USKAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	
	if(Attribute == GetMaxMoveSpeedAttribute())
	{
		AdjustAttributeForMaxChange(MoveSpeed, MaxMoveSpeed, NewValue, GetMoveSpeedAttribute());
	}
	
	if(Attribute == GetMaxLaneSwitchSpeedAttribute())
	{
		AdjustAttributeForMaxChange(LaneSwitchSpeed, MaxLaneSwitchSpeed, NewValue, GetLaneSwitchSpeedAttribute());
	}
}

void USKAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	float DeltaValue = -1.f;
	float OverridenValue = -1.f;

	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Override)
	{
		OverridenValue = Data.EvaluatedData.Magnitude;
	}
	

	ASKCharacter* TargetCharacter = nullptr;

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* TargetActor = nullptr;
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter = Cast<ASKCharacter>(TargetActor);
	}

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		if (TargetCharacter)
		{
			TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
		}
	}

	if(Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
	{
		SetMoveSpeed(FMath::Clamp(GetMoveSpeed(), 0.0f, GetMaxMoveSpeed()));

		if (TargetCharacter)
		{
			TargetCharacter->HandleMoveSpeedChanged(DeltaValue, OverridenValue, SourceTags);
		}
	}

	if(Data.EvaluatedData.Attribute == GetLaneSwitchSpeedAttribute())
	{
		SetLaneSwitchSpeed(FMath::Clamp(GetLaneSwitchSpeed(), 0.0f, GetMaxLaneSwitchSpeed()));

		if (TargetCharacter)
		{
			TargetCharacter->HandleLaneSwitchSpeedChanged(DeltaValue, SourceTags);
		}
	}
}

void USKAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute, float NewMaxValue,
	const FGameplayAttribute& EffectedAttributeProperty) const
{
	UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComponent)
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		const float NewDelta = (CurrentMaxValue > 0.f)
		? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue
		: NewMaxValue;

		AbilityComponent->ApplyModToAttributeUnsafe(EffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void USKAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USKAttributeSet, Health, OldValue);
}

void USKAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USKAttributeSet, MaxHealth, OldValue);
}

void USKAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USKAttributeSet, MoveSpeed, OldValue);
}

void USKAttributeSet::OnRep_MaxMoveSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USKAttributeSet, MaxMoveSpeed, OldValue);
}

void USKAttributeSet::OnRep_LaneSwitchSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USKAttributeSet, LaneSwitchSpeed, OldValue);
}

void USKAttributeSet::OnRep_MaxLaneSwitchSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USKAttributeSet, MaxLaneSwitchSpeed, OldValue);
}
