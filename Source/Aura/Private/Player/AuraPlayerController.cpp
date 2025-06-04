// Copyright Yocosi


#include "Player/AuraPlayerController.h"

#include <iostream>

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"

AAuraPlayerController::AAuraPlayerController()
{
	// To replicate/send down changes that took place to the server to all the clients
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// Assert that the AuraContext pointer is set. If not => Crash
	check(AuraContext);

	// Retrieve the Enhanced Input Local Player Subsystem associated with this PlayerController's LocalPlayer.
	// ULocalPlayer::GetSubsystem<T> returns the instance of the requested subsystem (here UEnhancedInputLocalPlayerSubsystem).
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem)
	
	// Add the input mapping context (AuraContext) with priority 0 to this subsystem.
	// This makes all the input bindings defined in AuraContext active as soon as the game starts.
	Subsystem->AddMappingContext(AuraContext, 0);

	// Show the mouse cursor (useful for UI interaction or debugging).
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// Prepare the input mode to allow both gameplay and UI interaction.
	// FInputModeGameAndUI enables input for the game (camera movement, character movement, etc.)
	// while still capturing the mouse for UI elements (menus, buttons, etc.).
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);

	// Apply the configured input mode to this PlayerController.
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AAuraPlayerController::Jump);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::Jump(const FInputActionValue& InputActionValue)
{
	if (ACharacter* ControlledCharacter = GetPawn<ACharacter>())
	{
		ControlledCharacter->Jump();
	}
}
