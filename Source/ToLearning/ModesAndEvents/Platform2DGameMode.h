// Create by Davidson

#pragma once

#include "CoreMinimal.h"
#include "EventManager.h"
#include "GameFramework/GameModeBase.h"
#include "Platform2DGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOLEARNING_API APlatform2DGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY()
	UEventManager* EventManager;
	
public:
	UEventManager* GetEventManager();

	APlatform2DGameMode();
	
};
