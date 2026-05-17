// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UAuraUserWidget;
class UOverlapWidgetController;
/**
 * 
 */
UCLASS()
class AURA_GAME_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:
	
	
	UOverlapWidgetController* GetOverlapWidgetController(const FWidgetControllerParams& Params);
	UAttributeMenuWidgetController* GetAttributeMenuController(const FWidgetControllerParams& Params);
	
	//初始化相关的widgetController,
	void InitOverlap(APlayerController* PC,APlayerState* PS,UAbilitySystemComponent* Asc,UAttributeSet* As);
	
	
protected:
	
	
private:
	
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> MainWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> MainWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UOverlapWidgetController> OverlapWidgetController;
	
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlapWidgetController> OverlapWidgetControllerClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeWidgetControllerClass;
};
