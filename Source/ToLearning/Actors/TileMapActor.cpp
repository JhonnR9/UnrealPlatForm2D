// Fill out your copyright notice in the Description page of Project Settings.


#include "TileMapActor.h"
#include "PaperTileMap.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ATileMapActor::ATileMapActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TileMapComponent = CreateDefaultSubobject<UEnhancedTileMapComponent>(TEXT("TileMap"));

	
	static ConstructorHelpers::FObjectFinder<UPaperTileMap> TileMapAsset(TEXT("/Game/Assets/TileMaps/Level_01.Level_01"));
	if (TileMapAsset.Succeeded()) {
		TileMapComponent->SetTileMap(TileMapAsset.Object);
	}


}

// Called when the game starts or when spawned
void ATileMapActor::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void ATileMapActor::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

UEnhancedTileMapComponent* ATileMapActor::GetEnhancedTileMapComponent() {
	return TileMapComponent;
}


