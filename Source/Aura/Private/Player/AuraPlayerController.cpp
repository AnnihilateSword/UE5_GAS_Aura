// Copyright AnnihilateSword.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/HighlightInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(m_AuraContext);

	if (IsLocalController())
	{
		// Add Aura Input Mapping Context.
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		if (EnhancedInputLocalPlayerSubsystem)
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(m_AuraContext, 0);
		}
	}

	// Set Mouse Cursor and Input Mode.
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Type::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(m_MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	}
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// 光标追踪检测
	CursorTrace();
}

void AAuraPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D InputAxisVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHitResult;
	// 第二个参数：不追踪复杂碰撞，只追踪简单碰撞
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHitResult);
	if (!CursorHitResult.bBlockingHit)
		return;

	m_LastActor = m_ThisActor;
	m_ThisActor = CursorHitResult.GetActor();

	/**
	 * 光标的 line 追踪，分几种情况：
	 * 1. m_LastActor is null && m_ThisActor is null
	 *		- Do nothing.
	 * 2. m_LastActor is null && m_ThisActor is valid
	 *		- Highligh m_ThisActor.
	 * 3. m_LastActor is valid && m_ThisActor is null
	 *		- UnHighligh m_LastActor.
	 * 4. Both actors are valid, but m_LastActor != m_ThisActor
	 *		- UnHighligh m_LastActor, and Highligh ThisActor.
	 * 5. Both actors are valid, and m_LastActor == m_ThisActor
	 *		- Do nothing.
	 */
	if (m_LastActor == nullptr)
	{
		if (m_ThisActor != nullptr)
		{
			// Case 2
			m_ThisActor->HighlightActor();
		}
		else
		{
			// Case 1: Do nothing
		}
	}
	else
	{
		if (m_ThisActor != nullptr)
		{
			if (m_LastActor != m_ThisActor)
			{
				// Case 4
				m_LastActor->UnHighlightActor();
				m_ThisActor->HighlightActor();
			}
			else
			{
				// Case 5: Do nothing
			}
		}
		else
		{
			// Case 3
			m_LastActor->UnHighlightActor();
		}
	}
}
