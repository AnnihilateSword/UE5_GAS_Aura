// Copyright AnnihilateSword.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveGameplayEffectHandle.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
struct FActiveGameplayEffectHandle;
class UGameplayEffect;

// 效果应用策略
UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

// 效果移除策略
UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta = (AllowPrivateAccess = "true"))
	bool m_bDestroyOnEffectRemoval = false;
	
	/** 立即生效 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> m_InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta = (AllowPrivateAccess = "true"))
	EEffectApplicationPolicy m_InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	/** 在一段时间内持续有效，引擎帮我们做了计时器的工作 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> m_DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta = (AllowPrivateAccess = "true"))
	EEffectApplicationPolicy m_DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	/** 无限效果，如果要移除效果需要删除 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> m_InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta = (AllowPrivateAccess = "true"))
	EEffectApplicationPolicy m_InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta = (AllowPrivateAccess = "true"))
	EEffectRemovalPolicy m_InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> m_ActiveEffectHandles;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta = (AllowPrivateAccess = "true"))
	float m_ActorLevel = 1.0f;
};
