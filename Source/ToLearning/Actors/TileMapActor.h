// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToLearning/Components/EnhancedTileMapComponent.h"

#include "TileMapActor.generated.h"

UCLASS()
class TOLEARNING_API ATileMapActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UEnhancedTileMapComponent* TileMapComponent;

public:
	// Sets default values for this actor's properties
	ATileMapActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable)
	UEnhancedTileMapComponent* GetEnhancedTileMapComponent();
	
};
