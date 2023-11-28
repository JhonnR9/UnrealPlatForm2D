// Create by Davidson


#include "EnhancedTileMapComponent.h"
#include "PaperTileMap.h"
#include "Kismet/GameplayStatics.h"
#include "ToLearning/ModesAndEvents/Platform2DGameMode.h"

UEnhancedTileMapComponent::UEnhancedTileMapComponent() {
	
}

void UEnhancedTileMapComponent::BeginPlay() {
	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld())) {
		if (APlatform2DGameMode* Platform2DGameMode = Cast<APlatform2DGameMode>(GameMode)) {
			if (UEventManager* Events = Platform2DGameMode->GetEventManager()) {
				FVector Position = GetComponentLocation();
				Events->GetOnTileMapSpawn()->Broadcast(FVector2d(Position.X, Position.Z), TileMapSize, TileSize);
			}
		}
	}
	
	Super::BeginPlay();
	
}

void UEnhancedTileMapComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	
}

bool UEnhancedTileMapComponent::SetTileMap(UPaperTileMap* NewTileMap) {
	const int32 MapWidth = NewTileMap->MapWidth;
	const int32 MapHeight = NewTileMap->MapHeight;

	const int32 TileWidth = NewTileMap->TileWidth;
	const int32 TileHeight = NewTileMap->TileHeight;

	const float PixelForUnreal = NewTileMap->PixelsPerUnrealUnit;

	const float ConvertedTileMapWidth = MapWidth * (TileWidth / PixelForUnreal);
	const float ConvertedTileMapHeight = MapHeight * (TileHeight / PixelForUnreal);

	TileMapSize = FVector2D(ConvertedTileMapWidth, ConvertedTileMapHeight);
	TileSize = FVector2D((TileWidth / PixelForUnreal), TileHeight / PixelForUnreal);
	
	return Super::SetTileMap(NewTileMap);
}
