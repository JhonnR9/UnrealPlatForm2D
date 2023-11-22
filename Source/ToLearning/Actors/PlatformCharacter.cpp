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

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeRotation(CameraRotationOffset);
	SpringArmComponent->TargetArmLength = SpringArmLength;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
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
		TileMapActor = Cast<ATileMapActor>( FoundActors[0]);
		TileMapComponent = TileMapActor->GetEnhancedTileMapComponent();
		
		UE_LOG(LogTemp, Warning, TEXT("%s"), *TileMapComponent->GetConvertedTileMapSize().ToString());

	}
	
}

void APlatformCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	Flip();

	FVector CameraLocation = CameraComponent->GetComponentLocation();
	FVector CameraMin = TileMapActor->GetActorLocation();
	FVector2d TileMapSize = TileMapComponent->GetConvertedTileMapSize();
	FVector CameraMax = FVector(TileMapSize.X +  CameraMin.X, CameraLocation.Y, TileMapSize.Y + CameraMin.Z);

	FVector NewCameraLocation = FMath::Clamp(CameraLocation,CameraMin, CameraMax);
	CameraComponent->SetWorldLocation(NewCameraLocation);

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
