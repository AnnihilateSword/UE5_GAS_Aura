// Copyright AnnihilateSword.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

protected:
	/** 每当我们为给定的 Widget 设置 WidgetController 时，我们也会调用此函数 */
	// 我们在项目中只会有一个 OverlayWidgetController，相当于单例。所以放在 HUD 中是挺合适的。
	// 我们将这个 Controller 作为 Controller 层，各个 AuraUserWidget 共用
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

public:
	// 这里使用的时候转换成具体的 WidgetController 类
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> m_WidgetController;
};
