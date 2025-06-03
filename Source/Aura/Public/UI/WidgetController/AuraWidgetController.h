// Copyright AnnihilateSword.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;


USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};


/**
 * 
 */
// 这里注意添加 Blueprintable, BlueprintType
UCLASS(Blueprintable, BlueprintType)
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	/** 该函数将在 AAuraHUD::InitOverlay 中 OverlayWidget 设置了 WidgetController 之后调用 */
	virtual void BroadcastInitialValues();

	/** 为所有依赖 Widget 绑定回调 */
	virtual void BindCallbacksToDependences();
	
protected:
	/** WidgetContoller 从下面这4个关键变量中获取数据 */
	//~ Begin Model Data.
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> m_PlayerController = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> m_PlayerState = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> m_AbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> m_AttributeSet = nullptr;
	//~ End Model Data.
};
