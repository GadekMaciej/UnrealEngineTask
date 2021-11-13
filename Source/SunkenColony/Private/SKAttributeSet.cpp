// Fill out your copyright notice in the Description page of Project Settings.


#include "SKAttributeSet.h"

#include "Net/UnrealNetwork.h"

USKAttributeSet::USKAttributeSet()
{
}

void USKAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USKAttributeSet, Health);
}

void USKAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USKAttributeSet, Health, OldValue);
}
