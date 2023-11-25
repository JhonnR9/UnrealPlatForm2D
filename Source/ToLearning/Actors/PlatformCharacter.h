// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "InputAction.h"
#include "EnhancedInputSubsystemInterface.h"
#include "InputActionValue.h"
#include "TileMapActor.h"
#include "../Components/EnhancedTileMapComponent.h"
#include "ToLearning/Components/Camera2DComponent.h"
#include "ToLearning/Components/SpringArm2DComponent.h"

#include "PlatformCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TOLEARNING_API APlatformCharacter : public APaperCharacter
{
	GENERATED_BODY()

	//* Inputs */
	UPROPERTY(VisibleAnywhere, Category="Inputs")
	UInputAction* MoveInputAction;

	UPROPERTY(VisibleAnywhere, Category="Inputs")
	UInputAction* JumpInputAction;

	UPROPERTY(VisibleAnywhere, Category="Inputs")
	UInputMappingContext* DefaultContext;
	
	/* Inputs Events */
	void Move(const FInputActionValue& Value);



	UPROPERTY(VisibleAnywhere)
	ATileMapActor* TileMapActor;
	
	void Flip();
	
protected:
	// Components
	UPROPERTY(VisibleAnywhere)
	USpringArm2DComponent* SpringArmComponent;
	
	UPROPERTY(EditAnywhere)
	UCamera2DComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	UEnhancedTileMapComponent* TileMapComponent;

public:
	APlatformCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
