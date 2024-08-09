// Copyright Epic Games, Inc. All Rights Reserved.

#include "D3_Project_MuPoGameMode.h"
#include "D3_Project_MuPoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AD3_Project_MuPoGameMode::AD3_Project_MuPoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

