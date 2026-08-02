// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AuraAbilityInfo.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
}

void USpellMenuWidgetController::BindCallbackToDependencies()
{

	GetAuraAbilitySystemComponent()->OnAbilityStatusDelegate.AddLambda(
		[this](const FGameplayTag& AbilityTag,const FGameplayTag& AbStatusTag)
		{
			if (AbilityInfo)
			{
				FAbilityInfo Info = AbilityInfo.Get()->FindAbilityInfoByTag(AbilityTag);
				Info.StatusTag = AbStatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		}
	);
}
