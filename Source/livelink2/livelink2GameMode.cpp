// Copyright Epic Games, Inc. All Rights Reserved.

#include "livelink2GameMode.h"
#include "livelink2Character.h"
#include "UObject/ConstructorHelpers.h"

Alivelink2GameMode::Alivelink2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
