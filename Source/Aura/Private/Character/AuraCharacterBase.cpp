// Copyright AnnihilateSword.


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	m_Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	m_Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	m_Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return m_AbilitySystemComponent;
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandl = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandl.Data.Get(), GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	// 因为次要属性是根据主要属性驱动的，我们先初始化主要属性
	ApplyEffectToSelf(m_DefaultPrimaryAttributesEffect, 1.0f);
	ApplyEffectToSelf(m_DefaultSecondaryAttributesEffect, 1.0f);
	// 这里有些属性会依赖主要属性，比如我们先设置 m_MaxHealth 和 m_MaxMana 然后再把 m_Health 和 m_Mana 设置跟它们相等
	ApplyEffectToSelf(m_DefaultVitalAttributesEffect, 1.0f);
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(m_AbilitySystemComponent);
	if (!HasAuthority())
		return;

	AuraASC->AddCharacterAbilities(m_StartupAbilities);
}

FVector AAuraCharacterBase::GetCombatSocketLocation()
{
	check(m_Weapon);
	return m_Weapon->GetSocketLocation(m_WeaponTipSocketName);
}
