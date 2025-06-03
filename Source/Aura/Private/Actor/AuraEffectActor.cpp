// Copyright AnnihilateSword.


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	/**
	 * 1. 从指定 Actor 获取 ASC；
	 * 2. ASC 创建 EffectContext 并添加 SourceObject；
	 * 3. ASC 使用 EffectContext 和 GameplayEffectClass 创建 GameplayEffectSpecHandle；
	 * 4. ASC 调用 ApplyGameplayEffectSpecToSelf 应用 Effect；
	 */
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!IsValid(TargetASC))
		return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, m_ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && m_InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		// 我们添加这个是为了后续可以在合适的时机可以删除效果
		m_ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (m_InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, m_InstantGameplayEffectClass);
	}
	if (m_DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, m_DurationGameplayEffectClass);
	}
	// 如果检查通过，重叠时应用无限效果
	if (m_InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, m_InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (m_InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, m_InstantGameplayEffectClass);
	}
	if (m_DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, m_DurationGameplayEffectClass);
	}
	// 如果检查通过，离开重叠时应用无限效果
	if (m_InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, m_InfiniteGameplayEffectClass);
	}
	
	// 如果检查通过，离开重叠时移除无限效果
	if (m_InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC))
			return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TPair<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : m_ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				// 移除指定的 GameplayEffect，第二个参数默认是 -1，会移除所有堆栈
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			m_ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}
