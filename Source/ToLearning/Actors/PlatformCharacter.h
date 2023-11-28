// Create by Davidson

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "InputAction.h"
#include "EnhancedInputSubsystemInterface.h"
#include "InputActionValue.h"
#include "PaperFlipbook.h"
#include "TileMapActor.h"
#include "ToLearning/ModesAndEvents/EventManager.h"
#include "ToLearning/Components/Camera2DComponent.h"

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
	
	UPROPERTY(EditAnywhere, Category="Inputs")
	UInputAction* AttackInputAction;

	UPROPERTY(VisibleAnywhere, Category="Inputs")
	UInputMappingContext* DefaultContext;
	
	/* Inputs Events */
	void Move(const FInputActionValue& Value);

	UEventManager* EventManager;
	UFUNCTION()
	void Attack();

	UPROPERTY(VisibleAnywhere)
	ATileMapActor* TileMapActor;
	
	void Flip();
	void ChangeAnimations();
	void SetAnimation(FString AninName, bool Looping = true);
	
protected:
	// Components
	UPROPERTY(EditAnywhere)
	UCamera2DComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	TMap<FString, UPaperFlipbook*> MapAnimations;

public:
	APlatformCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
