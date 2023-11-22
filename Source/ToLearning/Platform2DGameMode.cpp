// Fill out your copyright notice in the Description page of Project Settings.


#include "Platform2DGameMode.h"
#include "Actors/PlatformCharacter.h"

APlatform2DGameMode::APlatform2DGameMode() {
	DefaultPawnClass = APlatformCharacter::StaticClass();
}
