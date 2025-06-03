// Copyright AnnihilateSword.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UOverlayWidgetController;
class UAuraUserWidget;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	/** 如果有 m_OverlayWidgetController 则返回 m_OverlayWidgetController，如果没有就重新创建一个并存在 m_OverlayWidgetController */
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	/** 
	 * 初始化 OverlayWidget
	 * 没有放在 BeginPlay 中是因为，在 BeginPlay 可能有些值未初始化
	 * 决定放在哪里调用 InitOverlay：我们什么时候知道它们都已经用有效数据初始化了？
	 * 
	 * 一个很好的地方是在 AAuraCharacter::InitAbilityActorInfo()，所以我们会在这里调用该函数
	 */
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
public:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> m_OverlayWidget;

	// HUD 存储 Overlay Widget 控制器，这类似一个单例，游戏中只有一个
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> m_OverlayWidgetController = nullptr;
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> m_OverlayWidgetClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> m_OverlayWidgetControllerClass;
};
