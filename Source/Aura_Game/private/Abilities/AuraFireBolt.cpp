// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AuraFireBolt.h"
#include "UAuraGameplayTags.h"


FString UAuraFireBolt::GetDescriptionCurrent(int32 Level)
{
	const float ManaCost = GetManaCost(Level); 
	float Damage = GetDamageByTag(Level,FUAuraGameplayTags::Get().DamageTag_Fire);
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT("火焰箭\n发射对目标造成伤害，并且有几率使对方燃烧。火焰箭当前等级%d,发射%d个，伤害造成%f点火焰伤害.\n火焰箭消耗%f法力值,冷却为%f秒"),Level,FMath::Max(Level,MumFireBolt),Damage,FMath::Abs(ManaCost),Cooldown);
}

FString UAuraFireBolt::GetNextDescription(int32 Level)
{
	const float ManaCost = GetManaCost(Level); 
	float Damage = GetDamageByTag(Level,FUAuraGameplayTags::Get().DamageTag_Fire);
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT("火焰箭\n发射对目标造成伤害，并且有几率使对方燃烧。火焰箭当前等级%d,发射%d个，伤害造成%f点火焰伤害.\n火焰箭消耗%f法力值,冷却为%f秒"),Level,FMath::Max(Level,MumFireBolt),Damage,FMath::Abs(ManaCost),Cooldown);
}