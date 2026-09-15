// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SpellMenuWidgetController.h"

#include "UAuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AuraAbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	OnSpellPointsChanged.Broadcast(GetAuraPlayerState()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbackToDependencies()
{
	GetAuraAbilitySystemComponent()->OnAbilityStatusDelegate.AddLambda(
		[this](const FGameplayTag& AbilityTag,const FGameplayTag& AbStatusTag,const int32& Level)
		{
			if (SelectedAbility.Ability == AbilityTag)
			{
				SelectedAbility.Status = AbStatusTag;
				bool SpendPointButton = false;
				bool EquipButton = false;
				ShouldEnableButtons(AbStatusTag,CurrentSpellPoint,SpendPointButton,EquipButton);
				FString CurrentDesc;
				FString NextDesc;
				GetAuraAbilitySystemComponent()->GetAllDescriptionInfo(SelectedAbility.Ability,CurrentDesc,NextDesc);
				SpellGlobeSelectionDelegate.Broadcast(SpendPointButton,EquipButton,CurrentDesc,NextDesc );
			}
			if (AbilityInfo)
			{
				FAbilityInfo Info = AbilityInfo.Get()->FindAbilityInfoByTag(AbilityTag);
				Info.StatusTag = AbStatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		}
	);
	GetAuraAbilitySystemComponent()->OnEquipAbility.AddUObject(this,&USpellMenuWidgetController::OnAbilityEquip);
	
	GetAuraPlayerState()->OnSpellPointChangeDelegate.AddLambda(
		[this](int32 InSpellPoints)
		{
			CurrentSpellPoint = InSpellPoints;
			bool SpendPointButton = false;
			bool EquipButton = false;
			ShouldEnableButtons(SelectedAbility.Status,CurrentSpellPoint,SpendPointButton,EquipButton);
			
			FString CurrentDesc;
			FString NextDesc;
			GetAuraAbilitySystemComponent()->GetAllDescriptionInfo(SelectedAbility.Ability,CurrentDesc,NextDesc);
			SpellGlobeSelectionDelegate.Broadcast(SpendPointButton,EquipButton,CurrentDesc,NextDesc);
			OnSpellPointsChanged.Broadcast(InSpellPoints);
		}
	);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (IsEquipButton)
	{
		const FGameplayTag& AbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.Ability).AbilityType;
		StopWaitOnEquipButtonDelegate.Broadcast(AbilityType);
		IsEquipButton = false; 
	}
	
	
	FUAuraGameplayTags GameplayTags = FUAuraGameplayTags::Get();
	
	FGameplayTag AbilityStatusTag;
	const int32 CurSpellPoint = GetAuraPlayerState()->GetSpellPoints();
	
	FGameplayAbilitySpec * AbilitySpec = GetAuraAbilitySystemComponent()->GetAbilitySpecFromTag(AbilityTag);
	const bool bTagIsValid = AbilityTag.IsValid();
	const bool bTagIsNone = AbilityTag.MatchesTag(GameplayTags.Ability_None);
	const bool bSpecValid = AbilitySpec != nullptr;
	
	if (!bTagIsValid || !bSpecValid || bTagIsNone)
	{
		AbilityStatusTag = GameplayTags.Ability_Status_Locked;
	}
	else
	{
		AbilityStatusTag = GetAuraAbilitySystemComponent()->GetStatusTagFromSpec(*AbilitySpec);
	}
	
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatusTag;
	CurrentSpellPoint = CurSpellPoint;
	
	FString CurrentDesc;
	FString NextDesc;
	GetAuraAbilitySystemComponent()->GetAllDescriptionInfo(SelectedAbility.Ability,CurrentDesc,NextDesc);
	
	bool SpendPointButton = false;
	bool EquipButton = false;
	
	ShouldEnableButtons(AbilityStatusTag,CurSpellPoint,SpendPointButton,EquipButton);
	SpellGlobeSelectionDelegate.Broadcast(SpendPointButton,EquipButton,CurrentDesc,NextDesc);
	
}

void USpellMenuWidgetController::SpendSpellPointButtonPressed()
{
	GetAuraAbilitySystemComponent()->ServerSpendPointButtonPressed(SelectedAbility.Ability);
}

void USpellMenuWidgetController::DeselectButton()
{
	if (IsEquipButton)
	{
		const FGameplayTag& AbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.Ability).AbilityType;
		StopWaitOnEquipButtonDelegate.Broadcast(AbilityType);
		IsEquipButton = false;
	}
	
	SelectedAbility.Ability = FUAuraGameplayTags::Get().Ability_None;
	SelectedAbility.Status = FUAuraGameplayTags::Get().Ability_Status_Locked;
	SpellGlobeSelectionDelegate.Broadcast(false,false,FString(),FString());
	
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag& AbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.Ability).AbilityType;
	WaitOnEquipButtonDelegate.Broadcast(AbilityType);
	IsEquipButton = true;
	
	FGameplayTag SelectedStatusTag = GetAuraAbilitySystemComponent()->GetStatusForAbilityTag(SelectedAbility.Ability);
	if (SelectedStatusTag.MatchesTagExact(FUAuraGameplayTags::Get().Ability_Status_Equipped))
	{
		SelectedSlot = GetAuraAbilitySystemComponent()->GetInputTagFromAbilityTag(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& Slot,const FGameplayTag& AbilityType)
{
	if (!IsEquipButton) return;
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.Ability).AbilityType;
	if (!SelectedAbilityType.MatchesTag(AbilityType)) return;
	
	GetAuraAbilitySystemComponent()->ServerEquipAbility(SelectedAbility.Ability,Slot);
	
}

void USpellMenuWidgetController::OnAbilityEquip(const FGameplayTag& AbilityTag, const FGameplayTag& Status,const FGameplayTag& Slot, const FGameplayTag& PreSlot)
{
	IsEquipButton = false;
	
	FAbilityInfo LastAbilityInfo;
	LastAbilityInfo.InputTag = PreSlot;
	LastAbilityInfo.AbilityTag = FUAuraGameplayTags::Get().Ability_None;
	LastAbilityInfo.StatusTag = FUAuraGameplayTags::Get().Ability_Status_Unlocked;
	
	AbilityInfoDelegate.Broadcast(LastAbilityInfo);
	
	FAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.Ability);
	Info.AbilityTag = AbilityTag;
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	
	AbilityInfoDelegate.Broadcast(Info);
	StopWaitOnEquipButtonDelegate.Broadcast(AbilityInfo->FindAbilityInfoByTag(SelectedAbility.Ability).AbilityType);
	SpellGlobeReasDelegate.Broadcast(AbilityTag);
	DeselectButton();
	
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& StatusTag, int32 CurrentSpellPoint,bool& IsSpendPointButton, bool& IsEquipButton)
{
	
	FUAuraGameplayTags GameplayTags = FUAuraGameplayTags::Get();
	
	
	if (StatusTag.MatchesTag(GameplayTags.Ability_Status_Equipped))
	{
		IsEquipButton = true;
		if (CurrentSpellPoint > 0)
		{
			IsSpendPointButton = true;
		}
		
	}
	else if (StatusTag.MatchesTag(GameplayTags.Ability_Status_Eligible))
	{
		IsEquipButton = false;
		if (CurrentSpellPoint > 0)
		{
			IsSpendPointButton = true;
		}
	}
	else if (StatusTag.MatchesTag(GameplayTags.Ability_Status_Unlocked))
	{
		IsEquipButton = true;
		if (CurrentSpellPoint > 0)
		{
			IsSpendPointButton = true;
		}
	}
	else if(StatusTag.MatchesTag(GameplayTags.Ability_Status_Locked))
	{
		IsEquipButton = false;
	}
}
