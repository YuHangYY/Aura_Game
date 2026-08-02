// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/AuraWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraAbilityInfo.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	PlayerState = Params.PlayerState;
	PlayerController = Params.PlayerController;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
}

void UAuraWidgetController::BindCallbackToDependencies()
{
	
}

void UAuraWidgetController::BroadcastAbilityInfo()
{
	if (!GetAuraAbilitySystemComponent()->bAbilityGiven) return;
	
	FForEachAbility BroadcastDelegate;
	
	BroadcastDelegate.BindLambda(
	  [this](const FGameplayAbilitySpec& AbilitySpec)
	  {
		  //根据获取的Spec 设置输入标签
	  	FAbilityInfo Info =  AbilityInfo->FindAbilityInfoByTag(GetAuraAbilitySystemComponent()->GetAbilityTagFromSpec(AbilitySpec));
	  	Info.InputTag = GetAuraAbilitySystemComponent()->GetInputTagFromSpec(AbilitySpec);
	  	Info.StatusTag = AuraAbilitySystemComponent->GetStatusTagFromSpec(AbilitySpec);
	  	AbilityInfoDelegate.Broadcast(Info);
	  }
	);
	GetAuraAbilitySystemComponent()->ForEachAbility(BroadcastDelegate);
}

AAuraPlayerController* UAuraWidgetController::GetAuraPC()
{
	if (AuraPlayerController == nullptr)
	{
		AuraPlayerController = Cast<AAuraPlayerController>(PlayerController);
	}
	return AuraPlayerController;
}

AAuraPlayerState* UAuraWidgetController::GetAuraPlayerState()
{
	if (AuraPlayerState == nullptr)
	{
		AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	}
	return AuraPlayerState;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraAbilitySystemComponent()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	}
	return AuraAbilitySystemComponent;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAttributeSet()
{
	if (AuraAttributeSet == nullptr)
	{
		AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	}
	return AuraAttributeSet;
}
