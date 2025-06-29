// Copyright AnnihilateSword.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/HighlightInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	m_Spline = CreateDefaultSubobject<USplineComponent>("Spline");
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

	if (UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent))
	{
		AuraInputComponent->BindAction(m_MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
		
		AuraInputComponent->BindAbilityActions(m_InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	}
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// 光标追踪检测
	CursorTrace();
	AutoRun();
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
	// 第二个参数：不追踪复杂碰撞，只追踪简单碰撞
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, m_CusorResult);
	if (!m_CusorResult.bBlockingHit)
		return;

	m_LastActor = m_ThisActor;
	m_ThisActor = m_CusorResult.GetActor();

	if (m_LastActor != m_ThisActor)
	{
		if (m_LastActor) m_LastActor->UnHighlightActor();
		if (m_ThisActor) m_ThisActor->HighlightActor();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	// 如果是鼠标左键
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		// 检查我们的 m_ThisActor 是否是一个有效的 Actor
		m_bTargeting = m_ThisActor ? true : false;
		m_bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	// 1. 如果不是鼠标左键，我们激活能力
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	// 2. 我们按下了鼠标左键，并且鼠标在目标上盘旋，我们激活能力
	if (m_bTargeting)
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	}
	// 3. 如果按下了鼠标左键，但是没有目标，我们关心我们的移动行为
	else
	{
		if (m_FollowTime <= m_ShortPressThreshold)
		{
			const APawn* ControlledPawn = GetPawn();
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), m_CachedDestination))
			{
				m_Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					m_Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}
				if (NavPath->PathPoints.Num() > 0)
				{
					m_CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					m_bAutoRunning = true;
				}
			}
		}
		m_FollowTime = 0.0f;
		m_bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// 1. 如果不是鼠标左键，我们激活能力
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}

	// 2. 我们按下了鼠标左键，并且鼠标在目标上盘旋，我们激活能力
	if (m_bTargeting)
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
	// 3. 如果按下了鼠标左键，但是没有目标，我们关心我们的移动行为
	else
	{
		m_FollowTime += GetWorld()->GetDeltaSeconds();
		if (m_CusorResult.bBlockingHit) m_CachedDestination = m_CusorResult.ImpactPoint;

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (m_CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (m_AuraAbilitySystemComponent == nullptr)
	{
		m_AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return m_AuraAbilitySystemComponent;
}

void AAuraPlayerController::AutoRun()
{
	if (!m_bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = m_Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = m_Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - m_CachedDestination).Length();
		if (DistanceToDestination <= m_AutoRunAcceptanceRadius)
		{
			m_bAutoRunning = false;
		}
	}
}
