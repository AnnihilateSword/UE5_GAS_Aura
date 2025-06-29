// Copyright AnnihilateSword.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputConfig;
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

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UAuraAbilitySystemComponent* GetASC();

	void AutoRun();
	
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> m_AuraContext = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> m_MoveAction = nullptr;

	UPROPERTY()
	TScriptInterface<IHighlightInterface> m_LastActor;
	UPROPERTY()
	TScriptInterface<IHighlightInterface> m_ThisActor;
	FHitResult m_CusorResult;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> m_InputConfig = nullptr;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> m_AuraAbilitySystemComponent = nullptr;

	/**
	 * 点击移动
	 */
	/** 长按会在每帧调用 AddMovementInput，此时 m_bAutoRunning == false */
	FVector m_CachedDestination = FVector::ZeroVector;
	// 跟随光标的时间（判断我们是否只是短暂点击）
	float m_FollowTime = 0.0f;
	float m_ShortPressThreshold = 0.5f;
	// 短按的时候会自动生成平滑的样条曲线，当我们越来越接近目的地时我们也应该取消自动寻路
	bool m_bAutoRunning = false;
	// 我们是否瞄准了某个目标，比如鼠标左键点中了敌人（检查我们的 m_ThisActor）
	bool m_bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float m_AutoRunAcceptanceRadius = 50.0f;

	// 样条曲线允许我们根据一个世界位置（FVector）生成一条平滑的曲线
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> m_Spline;
};
