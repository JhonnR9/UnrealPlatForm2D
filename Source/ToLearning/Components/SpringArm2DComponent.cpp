// Fill out your copyright notice in the Description page of Project Settings.


#include "SpringArm2DComponent.h"

#include "Camera2DComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToLearning/Actors/TileMapActor.h"
#include "Engine/GameViewportClient.h"
#include "UnrealClient.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"


// Sets default values for this component's properties
USpringArm2DComponent::USpringArm2DComponent() {
	PrimaryComponentTick.bCanEverTick = true;

	const FRotator CameraRotationOffset = FRotator(0.f, 270.f, 0.f);
	constexpr float SpringArmLength = 700.f;

	SetRelativeRotation(CameraRotationOffset);
	TargetArmLength = SpringArmLength;
}

// Called when the game starts
void USpringArm2DComponent::BeginPlay() {
	Super::BeginPlay();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATileMapActor::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0) {
		ATileMapActor* TileMapActor = Cast<ATileMapActor>(FoundActors[0]);
		TileMapComponent = TileMapActor->GetEnhancedTileMapComponent();
	}
}


void USpringArm2DComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check if the first child component is a UCamera2DComponent
	if (USceneComponent* SceneComponent = GetChildComponent(0)) {
		if (UCamera2DComponent* Camera2DComponent = Cast<UCamera2DComponent>(SceneComponent)) {
			// Check if the parent is a UCapsuleComponent
			if (const UCapsuleComponent* Parent = Cast<UCapsuleComponent>(GetAttachParent())) {
				// Check if TileMapComponent is valid
				if (!TileMapComponent) return;

				// Get the game viewport and check its validity
				const UGameViewportClient* GameViewport = GetWorld()->GetGameViewport();
				if (!GameViewport) return;

				// Get the viewport and check its validity
				FViewport* Viewport = GameViewport->Viewport;
				if (!Viewport) return;

				// Get the size of the viewport
				const FVector2D ViewportSize = FVector2D(Viewport->GetSizeXY());

				// Calculate the aspect ratio
				const float AspectRatio = ViewportSize.X / ViewportSize.Y;

				// Get the position of the TileMapComponent
				const FVector TileMapStartPosition = TileMapComponent->GetComponentLocation();

				// Get the ortho size of the camera
				FVector ViewSize = Camera2DComponent->GetOrthoSize();
				ViewSize.Z = ViewSize.X / AspectRatio;

				// Get the position of the player
				const FVector PlayerPosition = Parent->GetComponentLocation();

				// Calculate the new relative location for the Camera2DComponent
				FVector NewRelativeLocation = FVector::Zero();

				const float TileWidth = TileMapComponent->GetConvertedTileSize().Y / 2;
				const float TileHeight = TileMapComponent->GetConvertedTileSize().X / 2;

				// Check distance for X-axis (left)
				const float DistancePlayerForTileMapX = FMath::Abs(PlayerPosition.X - TileMapStartPosition.X) +TileWidth;

				if (DistancePlayerForTileMapX <= ViewSize.X / 2) {
					NewRelativeLocation.Y = (ViewSize.X / 2) - (DistancePlayerForTileMapX);
				}

				// Check distance for Z-axis (top)
				const float PlayerForTopDistance = FMath::Abs(PlayerPosition.Z - (TileMapStartPosition.Z + TileHeight));

				if (PlayerForTopDistance <= ViewSize.Z / 2) {NewRelativeLocation.Z = -(PlayerPosition.Z - ((TileMapStartPosition.Z + TileHeight) - ViewSize.Z /2));}


			

				//check for X-axis (right)
				const FVector2D TileMapSize = TileMapComponent->GetConvertedTileMapSize();
				
				const float TileMapEndPositionX = TileMapStartPosition.X + TileMapSize.X - TileWidth;
				const float PlayerDistanceWalkedX = PlayerPosition.X - (TileMapStartPosition.X + TileWidth);
				const float PlayerForRightDistanceX = FMath::Abs(TileMapEndPositionX - PlayerDistanceWalkedX);
				const float HalfScreenDistanceX = ViewSize.X / 2;
				
				if (PlayerForRightDistanceX <= HalfScreenDistanceX) {
					const float DesiredDistance = TileMapEndPositionX - HalfScreenDistanceX;
					NewRelativeLocation.Y = DesiredDistance - PlayerPosition.X;
					
				}

				const float TileMapEndPositionZ = TileMapStartPosition.Z - TileMapSize.Y + TileWidth;
				const float PlayerDistanceWalkedZ = PlayerPosition.Z - (TileMapStartPosition.Z );
				const float PlayerForRightDistanceZ = FMath::Abs(TileMapEndPositionZ - PlayerDistanceWalkedZ);
				const float HalfScreenDistanceZ = ViewSize.Z / 2;
				
				if (PlayerForRightDistanceZ <= HalfScreenDistanceZ) {
					const float DesiredDistance = TileMapEndPositionZ + HalfScreenDistanceZ;
					NewRelativeLocation.Z = DesiredDistance - PlayerPosition.Z;
			
				}
				// Set the new relative location for the Camera2DComponent
				Camera2DComponent->SetRelativeLocation(NewRelativeLocation);
			}
		}
	}
}
