// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TileMapActor.h"
#include "Engine/GameViewportClient.h"



struct FCameraLensSettings;

APlatformCharacter::APlatformCharacter() {
	if (UPaperFlipbookComponent* FlipbookComponent = GetSprite()) {
		const FString FlipBookPath = TEXT("/Game/Assets/Hobbit/Flipbooks/hobbit__idle.hobbit__idle");
		const ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookAsset(*FlipBookPath);
		if (FlipbookAsset.Succeeded()) {
			FlipbookComponent->SetFlipbook(FlipbookAsset.Object);
			FlipbookComponent->SetRelativeLocation(FVector(0.f, 0.f, 35.f));
			FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement()) {
		CharacterMovementComponent->bConstrainToPlane = true;
		CharacterMovementComponent->bSnapToPlaneAtStart = true;
	}

	const FString MoveInputPath = TEXT("/Game/Inputs/IA_Move.IA_Move");
	const ConstructorHelpers::FObjectFinder<UInputAction> MoveInputAsset(*MoveInputPath);

	if (MoveInputAsset.Succeeded()) {
		MoveInputAction = MoveInputAsset.Object;
	}

	const FString JumpInputPath = TEXT("/Game/Inputs/IA_Jump.IA_Jump");
	const ConstructorHelpers::FObjectFinder<UInputAction> JumpInputAsset(*JumpInputPath);

	if (JumpInputAsset.Succeeded()) {
		JumpInputAction = JumpInputAsset.Object;
	}

	const FString ContextInputPath = TEXT("/Game/Inputs/IMC_Main.IMC_Main");
	const ConstructorHelpers::FObjectFinder<UInputMappingContext> ContextInputAsset(*ContextInputPath);

	if (ContextInputAsset.Succeeded()) {
		DefaultContext = ContextInputAsset.Object;
	}


	const FRotator CameraRotationOffset = FRotator(0.f, 270.f, 0.f);
	constexpr float SpringArmLength = 700.f;

	SpringArmComponent = CreateDefaultSubobject<USpringArm2DComponent>(TEXT("SPring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCamera2DComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	if (UCapsuleComponent* Capsule = GetCapsuleComponent()) {
		Capsule->SetCapsuleSize(30.f, 45.f);
	}
}

void APlatformCharacter::BeginPlay() {
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer()) {
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer)) {
				Subsystem->AddMappingContext(DefaultContext, 1);
			}
		}
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATileMapActor::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0) {
		TileMapActor = Cast<ATileMapActor>(FoundActors[0]);
		TileMapComponent = TileMapActor->GetEnhancedTileMapComponent();

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *TileMapComponent->GetConvertedTileMapSize().ToString());
	}
}

void APlatformCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	Flip();

	if (const UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport()) {
		FVector2d ViewportRect;
		ViewportClient->GetViewportSize(ViewportRect);

		FVector SpringArmLocation = GetActorLocation() + SpringArmComponent->GetRelativeLocation();
		SpringArmLocation.Y = 0;



		float OrthoHeight = CameraComponent->OrthoWidth / (ViewportRect.X / ViewportRect.Y);
		float OrthoWidth = CameraComponent->OrthoWidth;

	/*/	DrawDebugLine(GetWorld(), (SpringArmLocation - FVector(0, 0, OrthoHeight / 2-10)),
		              (SpringArmLocation + FVector(0, 0, OrthoHeight / 2-10)), FColor(0, 0, 255),
		              false, -1, 0, 2.0f);
		/*DrawDebugLine(GetWorld(), (SpringArmLocation - FVector(OrthoWidth / 2, 0, 0)),
		              (SpringArmLocation + FVector(OrthoWidth / 2-10, 0, 0)),
		              FColor(255, 0, 0), false, -1, 0, 2.0f);*/
	}
}

void APlatformCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		Input->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &APlatformCharacter::Move);
		Input->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &APlatformCharacter::Jump);
		Input->BindAction(JumpInputAction, ETriggerEvent::Canceled, this, &APlatformCharacter::StopJumping);
	}
}

void APlatformCharacter::Move(const FInputActionValue& Value) {
	const FVector WalkDirection = FVector(Value.Get<FVector2D>().X, 0.0f, 0.0f);
	GetCharacterMovement()->AddInputVector(WalkDirection, false);
}

void APlatformCharacter::Flip() {
	if (const UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement()) {
		if (UPaperFlipbookComponent* FlipbookComponent = GetSprite()) {
			const FVector CurrentVelocity = CharacterMovementComponent->GetLastUpdateVelocity();
			if (CurrentVelocity.X == 0) return;

			FVector CurrentScale = GetActorRelativeScale3D();
			CurrentScale.X = (CurrentVelocity.X > 0) ? CurrentScale.X : -CurrentScale.X;

			FlipbookComponent->SetRelativeScale3D(CurrentScale);
		}
	}
}
