// QYF Legal Description


#include "Player/AuraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(IMC_AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	// 多人模式下，使用check(Subsystem) SIMULATE角色一定会报错，所以得用if
	if (Subsystem)
	{
		Subsystem->AddMappingContext(IMC_AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector2D = InputActionValue.Get<FVector2D>();
	const FRotator MyControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, MyControlRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector2D.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector2D.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (CursorHit.bBlockingHit)
	{
		LastActor = ThisActor;
		ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
		/**
		 * Line trace from cursor. There are several scenarios
		 *  A. LastActor is null && ThisActor is null
		 *		- Do nothing
		 *	B. LastActor is null && ThisActor is valid
		 *		- Highlight ThisActor
		 *	C. LastActor is valid && ThisActor is null
		 *		- UnHighlight LastActor
		 *	D. Both actors are valid,but LastActor != ThisActor
		 *		- UnHighlight LastActor,and Highlight ThisActor
		 *	E. Both actors are valid,and are the same actor
		 *		- Do nothing
		 */
		if (LastActor != ThisActor)
		{
			if (LastActor) LastActor->UnHighlightActor();
			if (ThisActor) ThisActor->HighlightActor();
		}
	}
}
