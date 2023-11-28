// Create by Davidson


#include "Camera2DComponent.h"

#include "Engine/GameViewportClient.h"
#include "UnrealClient.h"
#include "Delegates/Delegate.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ToLearning/ModesAndEvents/Platform2DGameMode.h"

UCamera2DComponent::UCamera2DComponent() {
	SetProjectionMode(ECameraProjectionMode::Orthographic);
	SetOrthoWidth(1530);

	TileMapPosition = FVector2D::Zero();
	TileMapSize = FVector2D::Zero();
	TileSize = FVector2D::Zero();

	if (GetWorld()) {
		if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld())) {
			if (APlatform2DGameMode* Platform2DGameMode = Cast<APlatform2DGameMode>(GameMode)) {
				if (UEventManager* Events = Platform2DGameMode->GetEventManager()) {
					Events->GetOnTileMapSpawn()->AddDynamic(this, &UCamera2DComponent::HandleTileMapSpawn);
					Events->GetOnChangePlayerPosition()->AddDynamic(this, &UCamera2DComponent::LimitPositionOnTileMap);
				}
			}
		}
	}
}

void UCamera2DComponent::HandleTileMapSpawn(FVector2D NewPosition, FVector2D NewMapSize, FVector2D NewTileSize) {
	TileMapPosition = NewPosition;
	TileSize = NewTileSize;
	TileMapSize = NewMapSize;
}

void UCamera2DComponent::BeginPlay() {
	Super::BeginPlay();

	if (const UGameViewportClient* GameViewport = GetWorld()->GetGameViewport()) {
		if (const FViewport* Viewport = GameViewport->Viewport) {
			FVector2D ViewportSize = Viewport->GetSizeXY();
			SetAspectRatio(ViewportSize.X / ViewportSize.Y);
		}
	}
}

void UCamera2DComponent::LimitPositionOnTileMap(FVector2D PlayerPosition) {
	// Used for get center of screen position
	FVector2D ViewSize(OrthoWidth, OrthoWidth / AspectRatio);

	// The value of X and Z starts in the upper left corner, with the maximum of Z and the minimum of X
	const FVector2d DesiredStartLocation = FVector2d(TileMapPosition.X + (ViewSize.X / 2.f),
	                                                 TileMapPosition.Y - (ViewSize.Y / 2));
	FVector2D DesiredEndLocation = (TileMapPosition + TileMapSize) - (ViewSize / 2);

	// Position for ajuste camera in center os screen 
	const FVector2D DesiredStartRelativeLocation = (DesiredStartLocation - PlayerPosition);


	FVector NewRelativeLocation = FVector::Zero();
	// Check X axis And clamp in Min and Max
	if (PlayerPosition.X < DesiredStartLocation.X) {
		NewRelativeLocation.X = DesiredStartRelativeLocation.X;
	}
	else if (PlayerPosition.X > DesiredEndLocation.X) {
		NewRelativeLocation.X = (DesiredEndLocation.X - TileMapPosition.X) - PlayerPosition.X;
	}

	// Distance os Tile and player
	const float PlayerToTileMapOrigin = (TileMapPosition.Y - PlayerPosition.Y);
	
	// Check Z axis and clamp in Max, remember Z axis scale for negative vector
	if (PlayerToTileMapOrigin < TileMapPosition.Y - DesiredStartLocation.Y) {
		NewRelativeLocation.Z = DesiredStartRelativeLocation.Y;
	}

	
	// Finally set relative location
	SetRelativeLocation(NewRelativeLocation);
}
