// Copyright Epic Games, Inc. All Rights Reserved.

#include "LandScapesGameMode.h"
#include "LandScapesCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALandScapesGameMode::ALandScapesGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
