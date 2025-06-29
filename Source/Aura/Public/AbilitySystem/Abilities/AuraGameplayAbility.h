// Copyright AnnihilateSword.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// 如果这是一个启动能力（角色在开始就被赋予的能力），那么使用这个标签
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag m_StartupGameplayTag;
};
