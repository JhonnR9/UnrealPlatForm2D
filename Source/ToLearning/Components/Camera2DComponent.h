// Fill out your copyright notice in the Description page of Project Settings.

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

public:
	UCamera2DComponent();

	UFUNCTION(BlueprintCallable)
	FVector GetOrthoSize() const ;
};
