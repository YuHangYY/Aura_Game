// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

class UAuraWidgetController;
/**
 * 
 */
UCLASS()
class AURA_GAME_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//小部件控制器用于给父类实现的
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject *InWidgetController);
	
	//用于给子类蓝图实现初始化控制器的
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
	
	
protected:
};
