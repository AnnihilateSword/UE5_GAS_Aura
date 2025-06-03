// Copyright AnnihilateSword.


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	// 角色主相机初始化
	m_SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	m_SpringArm->SetupAttachment(RootComponent);
	m_SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	m_SpringArm->bUsePawnControlRotation = false;
	m_SpringArm->TargetArmLength = 750.0f;
	m_SpringArm->SocketOffset = FVector(0.0f, 0.0f, 0.0f);
	m_SpringArm->bEnableCameraLag = true;
	m_SpringArm->bInheritPitch = false;
	m_SpringArm->bInheritYaw = false;
	m_SpringArm->bInheritRoll = false;

	m_MainCamera = CreateDefaultSubobject<UCameraComponent>("MainCamera");
	m_MainCamera->SetupAttachment(m_SpringArm);

	// 移动组件初始化
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	check(CharacterMovementComponent);
	CharacterMovementComponent->bOrientRotationToMovement = true;
	CharacterMovementComponent->RotationRate = FRotator(0, 400.0f, 0);
	CharacterMovementComponent->bConstrainToPlane = true;
	CharacterMovementComponent->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init Ability Actor Info for [Server].
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init Ability Actor Info for [Client].
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	check(AuraPlayerState->GetAbilitySystemComponent());
	check(AuraPlayerState->GetAttributeSet());
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent());
	AuraAbilitySystemComponent->AbilityActorInfoSet();
	
	m_AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	m_AttributeSet = AuraPlayerState->GetAttributeSet();

	//~ Begin 初始化 Overlay Widget!!!
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, m_AbilitySystemComponent, m_AttributeSet);
		}
	}
	//~ End 初始化 Overlay Widget!!!
}
