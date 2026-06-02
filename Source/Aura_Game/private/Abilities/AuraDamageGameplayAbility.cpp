// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "character/AuraCharacter.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* Target)
{
	ICombatInterface* Actor =Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffect,Actor->GetPlayerLevel());
	
	for (TTuple<FGameplayTag, FScalableFloat> Pair:DamageTypes)
	{
		
		float DamageMagnitudePair = Pair.Value.GetValueAtLevel(Actor->GetPlayerLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle,Pair.Key,DamageMagnitudePair);
	}
	
	UAbilitySystemComponent* Asc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (Asc)
	{
		Asc->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
	}
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
