// Copyright AnnihilateSword.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class IHighlightInterface;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

private:
	/** 角色移动输入处理 */
	void Move(const FInputActionValue& Value);

	/** 光标信息追踪处理，高亮显示相关 Actor */
	void CursorTrace();
	
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> m_AuraContext = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> m_MoveAction = nullptr;

	UPROPERTY()
	TScriptInterface<IHighlightInterface> m_LastActor;
	UPROPERTY()
	TScriptInterface<IHighlightInterface> m_ThisActor;
};
