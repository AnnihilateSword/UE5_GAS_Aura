// Copyright AnnihilateSword.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTag, const FGameplayTagContainer& /*AssetTags*/);

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

protected:
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

public:
	// 用来通知给 WidgetController，当我们被应用了能力并改变各种标签时
	// 我们绑定到任何这个委托的类都将收到 TagContanier 这个参数
	FEffectAssetTag m_EffectAssetTag;
};
