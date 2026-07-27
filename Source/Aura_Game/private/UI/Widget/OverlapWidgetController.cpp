// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/OverlapWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/Data/AuraAbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"

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
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnXPChangeDelegate.AddUObject(this,&UOverlapWidgetController::OnXpChange); 
	AuraPlayerState->OnLevelChangeDelegate.AddLambda(
		[this](int32 NewValue)
		{
			OnPlayerLevelChangeDelegate.Broadcast(NewValue);
		}
	);
	
	
	
	//绑定回调函数为属性
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
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

void UOverlapWidgetController::OnXpChange(int32 NewXP)
{
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = AuraPlayerState->LeveLInfoPtr;
	
	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LeveLInformation.Num();
	
	if (Level <=MaxLevel && Level>0)
	{
		const int32 CurLevelUpRequirement = LevelUpInfo->LeveLInformation[Level].LevelUpRequirement;
		const int32 PreLevelUpRequirement = LevelUpInfo->LeveLInformation[Level-1].LevelUpRequirement;
			
		const int32 DeltaLevelUpRequirement = CurLevelUpRequirement - PreLevelUpRequirement; //获取的是最大范围经验值 900-300
		const int32 XPForLevel = NewXP - PreLevelUpRequirement;//获取当前范围经验值 500 - 300
		
		float XPPercent = static_cast<float>(XPForLevel) / static_cast<float>(DeltaLevelUpRequirement);
		
		OnXPPercentChangeDelegate.Broadcast(XPPercent);
	}
	
}
