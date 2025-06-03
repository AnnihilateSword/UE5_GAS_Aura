// Copyright AnnihilateSword.


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (m_OverlayWidgetController == nullptr)
	{
		m_OverlayWidgetController = NewObject<UOverlayWidgetController>(this, m_OverlayWidgetControllerClass);
		m_OverlayWidgetController->SetWidgetControllerParams(WCParams);

		// 2. 为所有依赖 Widget 绑定回调
		m_OverlayWidgetController->BindCallbacksToDependences();
		return m_OverlayWidgetController;
	}
	return m_OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(m_OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(m_OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));

	// 1. Create Aura User Widget
	m_OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), m_OverlayWidgetClass);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	// 3. 这里设置我们的 Overlay Widget Controller!!!
	m_OverlayWidget->SetWidgetController(WidgetController);

	// 4. 广播以初始化属性值
	WidgetController->BroadcastInitialValues();

	// Add m_OverlayWidget to Viewport!!!
	m_OverlayWidget->AddToViewport();
}
