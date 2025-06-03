// Copyright AnnihilateSword.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(m_AttributeSet);
	m_OnHealthChanged.Broadcast(AuraAttributeSet->Getm_Health());
	m_OnMaxHealthChanged.Broadcast(AuraAttributeSet->Getm_MaxHealth());
	m_OnManaChanged.Broadcast(AuraAttributeSet->Getm_Mana());
	m_OnMaxManaChanged.Broadcast(AuraAttributeSet->Getm_MaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependences()
{
	Super::BindCallbacksToDependences();

	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(m_AttributeSet);
	m_AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->Getm_HealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			m_OnHealthChanged.Broadcast(Data.NewValue);
		});
	
	m_AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->Getm_MaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			m_OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
	
	m_AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->Getm_ManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			m_OnManaChanged.Broadcast(Data.NewValue);
		});
	
	m_AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->Getm_MaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			m_OnMaxManaChanged.Broadcast(Data.NewValue);
		});

	// 当我们被应用了能力并改变各种标签时被通知到 WidgetController
	Cast<UAuraAbilitySystemComponent>(m_AbilitySystemComponent)->m_EffectAssetTag.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				// For example, say that Tag = Message.HealthPotion
				// "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
				const FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(m_MessageWidgetDataTable, Tag);
					m_MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);
}
