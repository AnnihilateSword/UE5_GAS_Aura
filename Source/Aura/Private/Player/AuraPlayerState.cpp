// Copyright AnnihilateSword.


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	// 设置更快的网络更新频率，因为挂载了 AbilitySystemComponent 和 AttributeSet
	// 这意味着每秒更新 100 次
	SetNetUpdateFrequency(100.0f);

	// 初始化 Ability System Component
	m_AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	m_AbilitySystemComponent->SetIsReplicated(true);
	// 设置复制模式
	m_AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// 初始化 Attribute Set
	m_AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, m_Level);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return m_AbilitySystemComponent;
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
}
