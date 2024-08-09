// Fill out your copyright notice in the Description page of Project Settings.


#include "OverworldGameMode.h"
#include "UObject/ConstructorHelpers.h"


AOverworldGameMode::AOverworldGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

