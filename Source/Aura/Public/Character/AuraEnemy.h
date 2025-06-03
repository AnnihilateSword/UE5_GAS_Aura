// Copyright AnnihilateSword.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/HighlightInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IHighlightInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	
protected:
	virtual void BeginPlay() override;

public:
	//~ Begin IHighlightInterface.
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~ End IHighlightInterface.

private:
	// 初始化 Ability Actor Info.
	virtual void InitAbilityActorInfo() override;
};
