// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"

void UAuraGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

FString UAuraGameplayAbility::GetDescriptionCurrent(int32 Level)
{
	return FString::Printf(TEXT("当前技能等级%d,"),Level);
}

FString UAuraGameplayAbility::GetNextDescription(int32 Level)
{
	return FString::Printf(TEXT(""));
}

FString UAuraGameplayAbility::GetLockedDescriptionCurrent()
{
	return FString::Printf(TEXT("当前技能未解锁请用法术点解锁!"));
}

float UAuraGameplayAbility::GetManaCost(int32 Level)
{
	float ManaCost = 0.0f;
	if (UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo& mod:CostEffect->Modifiers)
		{
			
			if (mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level,ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(int32 Level)
{
	float Cooldown = 0.0f;
	if (UGameplayEffect* CoolDownEffect = GetCooldownGameplayEffect())
	{
		CoolDownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(GetAbilityLevel(),Cooldown);
	}
	return Cooldown;
}
