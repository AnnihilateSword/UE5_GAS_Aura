// Copyright AnnihilateSword.


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	m_PlayerController = WCParams.PlayerController;
	m_PlayerState = WCParams.PlayerState;
	m_AbilitySystemComponent = WCParams.AbilitySystemComponent;
	m_AttributeSet = WCParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
}

void UAuraWidgetController::BindCallbacksToDependences()
{
}
