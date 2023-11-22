// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileMapComponent.h"
#include "EnhancedTileMapComponent.generated.h"

/**
 * 
 */
UCLASS()
class TOLEARNING_API UEnhancedTileMapComponent : public UPaperTileMapComponent
{
	GENERATED_BODY()

	UPROPERTY()
	FVector2D TileMapSize;
public:
	bool SetTileMap(UPaperTileMap* NewTileMap) override;


/**
 * Retrieves the dimensions of the TileMap converted to Unreal Units.
 *
 * @return The dimensions of the TileMap in Unreal Units (FVector2D).
 */
	UFUNCTION(BlueprintCallable)
	FVector2D GetConvertedTileMapSize() const;
};
