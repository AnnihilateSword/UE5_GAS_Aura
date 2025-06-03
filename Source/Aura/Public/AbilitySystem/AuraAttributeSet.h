// Copyright AnnihilateSword.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle;
	
	/** Source */
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	APlayerController* SourcePlayerController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	/** Target */
	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	APlayerController* TargetPlayerController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAuraAttributeSet();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Called just before any modification happens to an attribute. */
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	/**
	 * 在执行游戏效果后调用，用于修改某个属性的基础值。此后不能再进行任何修改。
	 * 注意，此函数仅在 “执行” 阶段被调用。例如，对某个属性 “基础值” 的修改。它不会在应用游戏效果（如持续 5 秒且增加 10 点移动速度的增益效果）时被调用。
	 */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData m_Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, m_Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData m_MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, m_MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData m_Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, m_Mana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData m_MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, m_MaxMana);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

private:
	void SetEffectProperties(const struct FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};
