// Copyright Epic Games, Inc. All Rights Reserved.

#include "TitanD2DemoGameMode.h"
#include "TitanD2DemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATitanD2DemoGameMode::ATitanD2DemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<ACharacter> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
