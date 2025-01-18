// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASGame250107GameMode.h"
#include "GASGame250107Character.h"
#include "UObject/ConstructorHelpers.h"

AGASGame250107GameMode::AGASGame250107GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
