// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class AURA_GAME_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbackToDependencies()override;
};
