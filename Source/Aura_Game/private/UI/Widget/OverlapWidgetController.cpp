// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/OverlapWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraAbilityInfo.h"

void UOverlapWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	HealthChangedSign.Broadcast(AuraAttributeSet->GetHealth());
	MaxHealthChangedSign.Broadcast(AuraAttributeSet->GetMaxHealth());
	ManaChangedSign.Broadcast(AuraAttributeSet->GetMana());
	MaxManaChangedSign.Broadcast(AuraAttributeSet->GetMaxMana());
	
}

void UOverlapWidgetController::BindCallbackToDependencies()
{
	//绑定回调函数为属性
	UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
	     [this](const FOnAttributeChangeData&Data)
	     {
	     	HealthChangedSign.Broadcast(Data.NewValue);
	     }
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		 [this](const FOnAttributeChangeData&Data)
		 {
		 	MaxHealthChangedSign.Broadcast(Data.NewValue);
		 }
		 );
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
		 [this](const FOnAttributeChangeData&Data)
		 {
		 	ManaChangedSign.Broadcast(Data.NewValue);
		 }
		 );
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		 [this](const FOnAttributeChangeData&Data)
		 {
		 	MaxManaChangedSign.Broadcast(Data.NewValue);
		 }
		 );
	
	if (UAuraAbilitySystemComponent* AuraAsc = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (AuraAsc->bAbilityGiven)
		{
			OnInitializeStartupAbilities(AuraAsc);
		}
		else
		{
			AuraAsc->FAbilityGivenDelegate.AddUObject(this,&UOverlapWidgetController::OnInitializeStartupAbilities);
		}
		
	
	    AuraAsc->EffectAssetTags.AddLambda(
		 [this](const FGameplayTagContainer& AssetTag)
		   {
			 for (auto Tag : AssetTag)
			 {
				 FGameplayTag tag = FGameplayTag::RequestGameplayTag("Message");
				 if (Tag.MatchesTag(tag))
				 {
					 FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
					 MessageWidgetRowDelegate.Broadcast(*Row);
				 }
	     		
	     	
			 }
		  }
	     );
	}
	
	
}

void UOverlapWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraAsc)
{
	if (!AuraAsc->bAbilityGiven) return;
	
	FForEachAbility BroadcastDelegate;
	
	BroadcastDelegate.BindLambda(
	  [this, AuraAsc](const FGameplayAbilitySpec& AbilitySpec)
	  {
	  	//根据获取的Spec 设置输入标签
		 FAbilityInfo Info =  AbilityInfo->FindAbilityInfoByTag(AuraAsc->GetAbilityTagFromSpec(AbilitySpec));
	  	 Info.InputTag = AuraAsc->GetInputTagFromSpec(AbilitySpec);
	  	AbilityInfoDelegate.Broadcast(Info);
	  }
	);
	AuraAsc->ForEachAbility(BroadcastDelegate);
}
