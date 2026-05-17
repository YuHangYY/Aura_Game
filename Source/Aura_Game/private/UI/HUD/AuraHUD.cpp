// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AttributeMenuWidgetController.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/Widget/OverlapWidgetController.h"

UOverlapWidgetController* AAuraHUD::GetOverlapWidgetController(const FWidgetControllerParams& Params)
{
	if (OverlapWidgetController == nullptr)
	{
		OverlapWidgetController = NewObject<UOverlapWidgetController>(this,OverlapWidgetControllerClass);
		OverlapWidgetController->SetWidgetControllerParams(Params);
		OverlapWidgetController->BindCallbackToDependencies();
	}
	return OverlapWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuController(const FWidgetControllerParams& Params)
{
	if (AttributeMenuController == nullptr)
	{
		AttributeMenuController = NewObject<UAttributeMenuWidgetController>(this,AttributeWidgetControllerClass);
		AttributeMenuController->SetWidgetControllerParams(Params);
		AttributeMenuController->BindCallbackToDependencies();
		
	}
	return AttributeMenuController;
}


void AAuraHUD::InitOverlap(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* Asc, UAttributeSet* As)
{
	MainWidget = CreateWidget<UAuraUserWidget>(GetWorld(),MainWidgetClass);
	const FWidgetControllerParams Params(PC,PS,Asc,As);
	UOverlapWidgetController* WidgetController = GetOverlapWidgetController(Params);
	UAttributeMenuWidgetController* AttributeMenuWidgetController = GetAttributeMenuController(Params);
	MainWidget->SetWidgetController(WidgetController);
	//广播内部初始值给绑定的Widget
	WidgetController->BroadcastInitialValues();
	MainWidget->AddToViewport();
	
	
}


