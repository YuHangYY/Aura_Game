 // Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityType.h"
#include "character/AuraCharacter.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* Target)
{
	if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffect,ICombatInterface::Execute_GetPlayerLevel(GetAvatarActorFromActorInfo()));
	
		
		
		
		float DamageMagnitudePair = Damage.GetValueAtLevel(ICombatInterface::Execute_GetPlayerLevel(GetAvatarActorFromActorInfo()));
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle,DamageType,DamageMagnitudePair);
		
	
		UAbilitySystemComponent* Asc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
		if (Asc)
		{
			Asc->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
		}
	}
	
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* Target) const
{
	FDamageEffectParams Param;
	Param.WorldContextObject = GetAvatarActorFromActorInfo();
	Param.DamageGameplayEffectClass = DamageEffect;
	Param.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Param.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	Param.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Param.AbilityLevel = GetAbilityLevel();
	Param.DamageType = DamageType;
	Param.DebuffChance = DebuffChance;
	Param.DebuffDamage = DebuffDamage;
	Param.DebuffFrequency = DebuffFrequency;
	Param.DebuffDamageDuration = DebuffDuration;
	return Param;

}

float UAuraDamageGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}

FTaggedMontage UAuraDamageGameplayAbility::GetRandomAttackMontageFromArray(const TArray<FTaggedMontage>& MontageArray)
{
	if (MontageArray.Num() > 0)
	{
		int size = FMath::RandRange(0,MontageArray.Num()-1);
		return  MontageArray[size];
	}
	return FTaggedMontage();
}
