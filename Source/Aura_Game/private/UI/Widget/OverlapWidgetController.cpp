// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/OverlapWidgetController.h"

#include "UAuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/Data/AuraAbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"

void UOverlapWidgetController::BroadcastInitialValues()
{
	HealthChangedSign.Broadcast(GetAuraAttributeSet()->GetHealth());
	MaxHealthChangedSign.Broadcast(GetAuraAttributeSet()->GetMaxHealth());
	ManaChangedSign.Broadcast(GetAuraAttributeSet()->GetMana());
	MaxManaChangedSign.Broadcast(GetAuraAttributeSet()->GetMaxMana());
	
}

void UOverlapWidgetController::BindCallbackToDependencies()
{
	GetAuraPlayerState()->OnXPChangeDelegate.AddUObject(this,&UOverlapWidgetController::OnXpChange); 
	GetAuraPlayerState()->OnLevelChangeDelegate.AddLambda(
		[this](int32 NewValue)
		{
			OnPlayerLevelChangeDelegate.Broadcast(NewValue);
		}
	);

	//绑定回调函数为属性
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetHealthAttribute()).AddLambda(
	     [this](const FOnAttributeChangeData&Data)
	     {
	     	HealthChangedSign.Broadcast(Data.NewValue);
	     }
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxHealthAttribute()).AddLambda(
		 [this](const FOnAttributeChangeData&Data)
		 {
		 	MaxHealthChangedSign.Broadcast(Data.NewValue);
		 }
		 );
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetManaAttribute()).AddLambda(
		 [this](const FOnAttributeChangeData&Data)
		 {
		 	ManaChangedSign.Broadcast(Data.NewValue);
		 }
		 );
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxManaAttribute()).AddLambda(
		 [this](const FOnAttributeChangeData&Data)
		 {
		 	MaxManaChangedSign.Broadcast(Data.NewValue);
		 }
		 );
	
	if (UAuraAbilitySystemComponent* AuraAsc = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		
		GetAuraAbilitySystemComponent()->OnEquipAbility.AddUObject(this,&UOverlapWidgetController::OnAbilityEquip);
		if (AuraAsc->bAbilityGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			AuraAsc->FAbilityGivenDelegate.AddUObject(this,&UOverlapWidgetController::BroadcastAbilityInfo);
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


void UOverlapWidgetController::OnXpChange(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetAuraPlayerState()->LeveLInfoPtr;
	
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

void UOverlapWidgetController::OnAbilityEquip(const FGameplayTag& AbilityTag, const FGameplayTag& Status,const FGameplayTag& Slot, const FGameplayTag& PreSlot)
{
	FAbilityInfo LastAbilityInfo;
	LastAbilityInfo.InputTag = PreSlot;
	LastAbilityInfo.AbilityTag = FUAuraGameplayTags::Get().Ability_None;
	LastAbilityInfo.StatusTag = FUAuraGameplayTags::Get().Ability_Status_Unlocked;
	
	AbilityInfoDelegate.Broadcast(LastAbilityInfo);
	
	FAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	
	AbilityInfoDelegate.Broadcast(Info);
}
