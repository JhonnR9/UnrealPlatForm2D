// Create by Davidson


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
#include "Engine/GameViewportClient.h"
#include "ToLearning/ModesAndEvents/Platform2DGameMode.h"


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
		CharacterMovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);
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
	
	const FString AttackInputPath = TEXT("/Game/Inputs/IA_Attack.IA_Attack");
	const ConstructorHelpers::FObjectFinder<UInputAction> AttackInputAsset(*AttackInputPath);

	if (AttackInputAsset.Succeeded()) {
		AttackInputAction = JumpInputAsset.Object;
	}

	const FString ContextInputPath = TEXT("/Game/Inputs/IMC_Main.IMC_Main");
	const ConstructorHelpers::FObjectFinder<UInputMappingContext> ContextInputAsset(*ContextInputPath);

	if (ContextInputAsset.Succeeded()) {
		DefaultContext = ContextInputAsset.Object;
	}

	CameraComponent = CreateDefaultSubobject<UCamera2DComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);
	CameraComponent->SetRelativeRotation(FRotator(0, 270, 0));
	CameraComponent->SetRelativeLocation(FVector(0, 150, 0));
	CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
	CameraComponent->SetOrthoWidth(1530);

	if (UCapsuleComponent* Capsule = GetCapsuleComponent()) {
		Capsule->SetCapsuleSize(30, 45);
	}

	if (GetWorld()) {
		if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld())) {
			if (APlatform2DGameMode* Platform2DGameMode = Cast<APlatform2DGameMode>(GameMode)) {
				EventManager = Platform2DGameMode->GetEventManager();
			}
		}
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
}

void APlatformCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	Flip();
	ChangeAnimations();

	if (!GetCharacterMovement()->GetLastUpdateVelocity().IsNearlyZero()) {
		if (EventManager) {
			const FVector Position = GetActorLocation();
			EventManager->GetOnChangePlayerPosition()->Broadcast(FVector2D(Position.X, Position.Z));
		}
	}
}

void APlatformCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		Input->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &APlatformCharacter::Move);
		Input->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &APlatformCharacter::Jump);
		Input->BindAction(AttackInputAction, ETriggerEvent::Triggered, this, &APlatformCharacter::Attack);
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

void APlatformCharacter::SetAnimation(FString AninName, bool Looping) {
	if (UPaperFlipbookComponent* FlipbookComponent = GetSprite()) {
		// If the animation ends, be sure to enable the loop to continue the animations
		if (!FlipbookComponent->IsPlaying()) {
			FlipbookComponent->SetLooping(true);
			FlipbookComponent->Play();
			
		}
		// Make sure the animation will end if it is of the non-loop type
		if (!FlipbookComponent->IsLooping()) {
			return;
		}
		
		if (UPaperFlipbook** Animation = MapAnimations.Find(AninName)) {
			// Check if the current animation is already the desired animation, and return if true
			if(FlipbookComponent->GetFlipbook()== *Animation) return;
			// Set the FlipbookComponent to the new animation and configure looping
			FlipbookComponent->SetFlipbook(*Animation);
			FlipbookComponent->SetLooping(Looping);

		}
		else {
			// Log a warning if the desired animation is not found in MapAnimations
			UE_LOG(LogTemp, Warning, TEXT("Animation %s not found in MapAnimations Please add it in Blueprint or c++ class."), *AninName);
		}
	}
}

void APlatformCharacter::ChangeAnimations() {
	if (const UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement()) {
		const FVector CurrentVelocity = CharacterMovementComponent->GetLastUpdateVelocity();

		if (CharacterMovementComponent->IsFalling()) {
			SetAnimation(TEXT("Jump"), false);
		}
		else if (!CurrentVelocity.IsNearlyZero()) {
			SetAnimation(TEXT("Run"));
		}
		else {
			
			SetAnimation(TEXT("Idle"));
		}
	}
}

void APlatformCharacter::Attack() {
	SetAnimation(TEXT("Attack"), false);
}

