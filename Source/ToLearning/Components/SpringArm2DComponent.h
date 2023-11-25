// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedTileMapComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SpringArm2DComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOLEARNING_API USpringArm2DComponent : public USpringArmComponent
{
	GENERATED_BODY()

	UPROPERTY()
	UEnhancedTileMapComponent* TileMapComponent;

public:
	// Sets default values for this component's properties
	USpringArm2DComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
