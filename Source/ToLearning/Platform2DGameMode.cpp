// Fill out your copyright notice in the Description page of Project Settings.


#include "Platform2DGameMode.h"
#include "Actors/PlatformCharacter.h"
#include "UObject/ConstructorHelpers.h"

APlatform2DGameMode::APlatform2DGameMode() {

	static ConstructorHelpers::FClassFinder<APawn> BP_PlatformCharacter(TEXT("/Game/Bluprints/BP_PlatformCharacter"));
	DefaultPawnClass = BP_PlatformCharacter.Class;
}
