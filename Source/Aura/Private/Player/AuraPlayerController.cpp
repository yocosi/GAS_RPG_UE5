// Copyright Yocosi


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"

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
