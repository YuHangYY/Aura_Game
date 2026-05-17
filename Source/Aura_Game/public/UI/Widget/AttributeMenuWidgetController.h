// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FGameplayAttribute;
class UAuraAttributeSet;
struct FGameplayTag;
class UAttributeInfo;
struct FAuraAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeMenuInfoDelegate, const FAuraAttributeInfo& ,Info);

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class AURA_GAME_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallbackToDependencies() override;
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() override;
	
	void BroadcastAttributeInfo(const FGameplayTag& tag,const FGameplayAttribute& Attribute )const;
	
	UPROPERTY(BlueprintAssignable)
	FAttributeMenuInfoDelegate AttributeInfoDelegate;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;
	
	
};
