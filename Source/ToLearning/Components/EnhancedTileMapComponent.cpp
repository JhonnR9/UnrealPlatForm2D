// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhancedTileMapComponent.h"
#include "PaperTileMap.h"

bool UEnhancedTileMapComponent::SetTileMap(UPaperTileMap* NewTileMap) {
	const int32 MapWidth = NewTileMap->MapWidth;
	const int32 MapHeight = NewTileMap->MapHeight;
	
	const int32 TileWidth = NewTileMap->TileWidth;
	const int32 TileHeight = NewTileMap->TileHeight;

	const float PixelForUnreal = NewTileMap->PixelsPerUnrealUnit;

	const float ConvertedTileMapWidth = MapWidth * (TileWidth / PixelForUnreal);
	const float ConvertedTileMapHeight = MapHeight * (TileHeight / PixelForUnreal);

	TileMapSize = FVector2D( ConvertedTileMapWidth, ConvertedTileMapHeight);
	TileSize = FVector2D((TileWidth / PixelForUnreal), TileHeight / PixelForUnreal);

	return Super::SetTileMap(NewTileMap);
}

FVector2D UEnhancedTileMapComponent::GetConvertedTileMapSize() const {
	return TileMapSize;
}

FVector2D UEnhancedTileMapComponent::GetConvertedTileSize() const {
	return TileSize;
}
