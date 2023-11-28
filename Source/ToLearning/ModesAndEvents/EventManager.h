// Create by Davidson

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EventManager.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTileMapSpawn, FVector2D, NewPosition, FVector2D, NewMapSize,
                                               FVector2D, NewTileSize);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangePlayerPosition, FVector2D, PlayerPosition);

UCLASS()
class TOLEARNING_API UEventManager : public UObject
{
	GENERATED_BODY()

public:
	FOnTileMapSpawn* GetOnTileMapSpawn();
	FOnChangePlayerPosition* GetOnChangePlayerPosition();
};
