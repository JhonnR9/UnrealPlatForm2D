// Create by Davidson

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
	
public:
	UEnhancedTileMapComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual bool SetTileMap(UPaperTileMap* NewTileMap) override;

private:
	UPROPERTY()
	FVector2D TileMapSize;
	UPROPERTY()
	FVector2D TileSize;
	
};
