// Copyright AnnihilateSword.


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	m_WidgetController = InWidgetController;
	WidgetControllerSet();
}
