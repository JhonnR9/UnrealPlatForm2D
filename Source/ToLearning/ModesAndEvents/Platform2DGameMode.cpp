// Create by Davidson


#include "Platform2DGameMode.h"

#include "ToLearning/Actors/PlatformCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"

APlatform2DGameMode::APlatform2DGameMode() {

	static ConstructorHelpers::FClassFinder<APawn> BP_PlatformCharacter(TEXT("/Game/Bluprints/BP_PlatformCharacter"));
	DefaultPawnClass = BP_PlatformCharacter.Class;
	EventManager = NewObject<UEventManager>(this, UEventManager::StaticClass(), TEXT("Events"));
}


UEventManager* APlatform2DGameMode::GetEventManager() {
	return EventManager;
}

