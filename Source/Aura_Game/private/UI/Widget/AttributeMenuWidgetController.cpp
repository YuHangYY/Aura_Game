// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/AttributeMenuWidgetController.h"

#include "UAuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

class UAuraAttributeSet;

void UAttributeMenuWidgetController::BindCallbackToDependencies()
{
	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	for (TPair<FGameplayTag, FGameplayAttribute(*)()>& pair:AS->TagToAttributeMapping)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(pair.Value()).AddLambda(
		[this,pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(pair.Key,pair.Value());
		}  
		);
	}
	
	GetAuraPlayerState()->OnAttributePointChangeDelegate.AddLambda(
	   [this](int32 AttributePoint)
	   {
		   AttributePointChangeDelegate.Broadcast(AttributePoint);
	   }
	);
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);
	for (auto&pair: GetAuraAttributeSet()->TagToAttributeMapping)
	{
		BroadcastAttributeInfo(pair.Key,pair.Value());
	}
	
	 
	AttributePointChangeDelegate.Broadcast(GetAuraPlayerState()->GetAttributePoints());
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& tag,const FGameplayAttribute& Attribute)const
{
	
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(tag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetAuraAbilitySystemComponent()->UpgradeAttribute(AttributeTag);
	
}


