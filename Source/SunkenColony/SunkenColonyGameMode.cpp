// Copyright Epic Games, Inc. All Rights Reserved.

#include "SunkenColonyGameMode.h"
#include "SunkenColonyCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASunkenColonyGameMode::ASunkenColonyGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
