// Copyright AnnihilateSword.


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// 初始化 Ability System Component
	m_AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	m_AbilitySystemComponent->SetIsReplicated(true);
	// 设置复制模式
	m_AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// 初始化 Attribute Set.
	m_AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	check(m_AbilitySystemComponent);

	InitAbilityActorInfo();
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	m_Weapon->SetRenderCustomDepth(true);
	m_Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	m_Weapon->SetRenderCustomDepth(false);
}

int32 AAuraEnemy::GetPlayerLevel()
{
	return m_Level;
}

void AAuraEnemy::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	// Init Ability Actor Info for [Server].
	m_AbilitySystemComponent->InitAbilityActorInfo(this, this);

	UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(m_AbilitySystemComponent);
	AuraAbilitySystemComponent->AbilityActorInfoSet();
}
