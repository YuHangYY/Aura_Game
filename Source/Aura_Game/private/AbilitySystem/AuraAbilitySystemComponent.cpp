// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "UAuraGameplayTags.h"
#include "Abilities/AuraGameplayAbility.h"
#include "Aura_Game/AuraLogChannels.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuraAbilitySystemComponent::ClientEffectApplied);
	
	const FUAuraGameplayTags& GameplayTags = FUAuraGameplayTags::Get();
}


void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities)
{
	//遍历角色拥有的数组，为每个技能创建登记表(spec)，并添加到ASC中，
	for (const TSubclassOf<UGameplayAbility> AbilityClass  : StartUpAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		if (const UAuraGameplayAbility* AuraGameplayAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraGameplayAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
	bAbilityGiven = true;
	FAbilityGivenDelegate.Broadcast(this);
	
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	//遍历所有可激活的技能
	for ( FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{   //找与输入对应标签的标签
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{    
			AbilitySpecInputPressed(Spec);
			if (!Spec.IsActive())
			{    //激活
				TryActivateAbility(Spec.Handle);
			}
		}
	}
	
}

void UAuraAbilitySystemComponent::AbilityInputTagRelease(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	//遍历所有可激活的技能
	for ( FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{   //找与输入对应标签的标签
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{     
			AbilitySpecInputReleased(Spec);
			
		}
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopedLock(*this);
	
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura,Error,TEXT("Failed to execute delegate in%hs"),__FUNCTION__);
		}
	}
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	
	if (!bAbilityGiven)
	{
		bAbilityGiven = true;
		FAbilityGivenDelegate.Broadcast(this);
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (const FGameplayTag& AbilityTag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (AbilityTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability"))))
			{
				return AbilityTag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Input"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}


