// Copyright AnnihilateSword.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UAuraUserWidget;
struct FOnAttributeChangeData;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	FText MessageText = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UAuraUserWidget> MessageWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	UTexture2D* MessageImage = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);

/**
 * 
 */
UCLASS()
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	/** 该函数将在 AAuraHUD::InitOverlay 中 OverlayWidget 设置了 WidgetController 之后调用 */
	virtual void BroadcastInitialValues() override;

	/** 为所有依赖 Widget 绑定回调 */
	virtual void BindCallbacksToDependences() override;

protected:
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
	
public:
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature m_OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature m_OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature m_OnManaChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature m_OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Message")
	FMessageWidgetRowSignature m_MessageWidgetRowDelegate;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> m_MessageWidgetDataTable;
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
