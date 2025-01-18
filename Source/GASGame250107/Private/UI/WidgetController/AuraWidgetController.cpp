// QYF Legal Description


#include "UI\WidgetController\AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComp = WidgetControllerParams.AbilitySystemComp;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitValues()
{
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}
