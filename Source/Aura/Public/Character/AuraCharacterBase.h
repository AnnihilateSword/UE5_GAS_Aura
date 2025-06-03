// Copyright AnnihilateSword.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	//~ Begin IAbilitySystemInterface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface.
	
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return m_AttributeSet; }

protected:
	// 初始化 Ability Actor Info.
	virtual void InitAbilityActorInfo();
	
protected:
	// 角色手持的武器
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> m_Weapon;

	// 这里在基类存储一份 AbilitySystemComponent 和 AttributeSet 但是不在基类中构造
	// 玩家会在 PlayerState 中构造，敌人 AI 会在敌人类中构造
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> m_AbilitySystemComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UAttributeSet> m_AttributeSet = nullptr;
};
