// Copyright AnnihilateSword.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//~ Begin IAbilitySystemInterface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface.
	
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return m_AttributeSet; }

	FORCEINLINE int32 GetPlayerLevel() const { return m_Level; }
	
protected:
	// 玩家会在 PlayerState 中构造 GAS 相关信息
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> m_AbilitySystemComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UAttributeSet> m_AttributeSet = nullptr;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 m_Level = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
};
