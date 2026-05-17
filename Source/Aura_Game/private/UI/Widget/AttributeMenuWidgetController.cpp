// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/AttributeMenuWidgetController.h"

#include "UAuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

class UAuraAttributeSet;

void UAttributeMenuWidgetController::BindCallbackToDependencies()
{
	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	for (auto& pair:AS->TagToAttributeMapping)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(pair.Value()).AddLambda(
		[this,pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(pair.Key,pair.Value());
		}  
		);
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);
	
	for (auto&pair: AuraAttributeSet->TagToAttributeMapping)
	{
		BroadcastAttributeInfo(pair.Key,pair.Value());
	}
	
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& tag,const FGameplayAttribute& Attribute)const
{
	
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(tag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}


