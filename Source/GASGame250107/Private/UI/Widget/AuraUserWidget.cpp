// QYF Legal Description


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* MyWidgetController)
{
	WidgetController = MyWidgetController;
	WidgetControllerSet();
}
