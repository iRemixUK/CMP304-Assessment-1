// Copyright Epic Games, Inc. All Rights Reserved.

#include "CMP304GameMode.h"
#include "CMP304Character.h"
#include "UObject/ConstructorHelpers.h"

ACMP304GameMode::ACMP304GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
