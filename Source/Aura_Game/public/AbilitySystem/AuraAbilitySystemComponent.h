// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraAbilitySystemComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& AssetTags);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven,UAuraAbilitySystemComponent*);
DECLARE_DELEGATE_OneParam(FForEachAbility,const FGameplayAbilitySpec&);

/**
 * 
 */
UCLASS()
class AURA_GAME_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	//初始化effect应用时调用委托把数据传过去
	void AbilityActorInfoSet();
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagRelease(const FGameplayTag& InputTag);
	
	void ForEachAbility(const FForEachAbility& Delegate);
	
	 void OnRep_ActivateAbilities()override;
	
	
	FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven FAbilityGivenDelegate;
	
	bool bAbilityGiven = false;
protected:
	UFUNCTION(Client,Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
