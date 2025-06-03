// Copyright AnnihilateSword.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	// 初始化 Ability Actor Info.
	virtual void InitAbilityActorInfo() override;
	
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> m_SpringArm = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> m_MainCamera = nullptr;
};
