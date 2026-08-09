// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UAuraGameplayTags.h"
#include "UI/Widget/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

struct FUAuraGameplayTags;
struct FGameplayTag;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectionSignature,bool,bSpendPointButtonEnabled,bool,bEquippedButtonEnabled,const FString& ,CurDesc,const FString& ,NextDesc);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitOnEquipButtonSignature,const FGameplayTag&,AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReasSignature,const FGameplayTag&,AbilityTag);
USTRUCT()
struct FSelectedAbility
{
	GENERATED_BODY()
	
	
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
	
};



UCLASS(Blueprintable,BlueprintType)
class AURA_GAME_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbackToDependencies()override;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChangeSign OnSpellPointsChanged;
	
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectionSignature SpellGlobeSelectionDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FWaitOnEquipButtonSignature WaitOnEquipButtonDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FWaitOnEquipButtonSignature StopWaitOnEquipButtonDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeReasSignature SpellGlobeReasDelegate;
	
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);
	
	UFUNCTION(BlueprintCallable)
	void SpendSpellPointButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void DeselectButton();
	
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& Slot,const FGameplayTag& AbilityType);
	
	void OnAbilityEquip(const FGameplayTag& AbilityTag,const FGameplayTag& Status,const FGameplayTag& Slot,const FGameplayTag& PreSlot);
private:
	static void ShouldEnableButtons(const FGameplayTag& StatusTag,int32 CurrentSpellPoint,bool& IsSpendPointButton,bool& IsEquipButton);
	
	FSelectedAbility SelectedAbility{FUAuraGameplayTags::Get().Ability_None,FUAuraGameplayTags::Get().Ability_Status_Locked};
	int32 CurrentSpellPoint = 0;
	
	bool IsEquipButton = false;
	FGameplayTag SelectedSlot = FGameplayTag();
};
