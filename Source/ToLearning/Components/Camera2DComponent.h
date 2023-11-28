// Create by Davidson

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Camera2DComponent.generated.h"

/**
 * 
 */
UCLASS()
class TOLEARNING_API UCamera2DComponent : public UCameraComponent
{
	GENERATED_BODY()

	FVector2D TileMapPosition;
	FVector2D TileMapSize;
	FVector2D TileSize;

	UFUNCTION()
	void LimitPositionOnTileMap(FVector2D PlayerPosition);
	
	UFUNCTION()
	void HandleTileMapSpawn(FVector2D NewPosition, FVector2D NewMapSize, FVector2D NewTileSize);
	
public:
	UCamera2DComponent();
	virtual void BeginPlay() override;

};
