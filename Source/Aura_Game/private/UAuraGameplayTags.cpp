// Fill out your copyright notice in the Description page of Project Settings.


#include "UAuraGameplayTags.h"
#include"GameplayTagsManager.h"

FUAuraGameplayTags FUAuraGameplayTags::GameplayTags;

void FUAuraGameplayTags::InitializeNativeGameplayTags()
{
	//primary
	GameplayTags.Attribute_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Strength"),FString("增加物理伤害"));
	GameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Intelligence"),FString("增加魔法伤害"));
	GameplayTags.Attribute_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Resilience"),FString("增加护甲和护甲穿透"));
	GameplayTags.Attribute_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Vigor"),FString("增加健康"));
	
	//Secondary
	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Armor"),FString("减少伤害"));
	GameplayTags.Attribute_Secondary_ArmorPenetration=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.ArmorPenetration"),FString("减少承受的伤害，提高格挡几率"));
	GameplayTags.Attribute_Secondary_BlockChance=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.BlockChance"),FString("格挡几率：受到攻击时触发格挡的概率"));
	GameplayTags.Attribute_Secondary_CriticalHitChance=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitChance"),FString("暴击几率：攻击造成暴击的概率"));
	GameplayTags.Attribute_Secondary_CriticalHitDamage=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitDamage"),FString("暴击伤害：暴击时额外造成的伤害"));
	GameplayTags.Attribute_Secondary_CriticalHitResistance=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitResistance"),FString("暴击抗性：降低被暴击的概率或暴击伤害"));
	GameplayTags.Attribute_Secondary_HealthRegeneration=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.HealthRegeneration"),FString("生命恢复：每秒恢复的生命值"));
	GameplayTags.Attribute_Secondary_ManaRegeneration=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.ManaRegeneration"),FString("法力恢复：每秒恢复的法力值"));
	GameplayTags.Attribute_Secondary_MaxHealth=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxHealth"),FString("最大生命值：角色可拥有的生命值上限"));
	GameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxMana"),FString("最大法力值：角色可拥有的法力值上限"));
	
	//Input
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.LMB"),FString("鼠标左键标签"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.RMB"),FString("鼠标右键标签"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.1"),FString("1键标签"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.2"),FString("2键标签"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.3"),FString("3键标签"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.4"),FString("4键标签"));
	
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"),FString("Damage"));
	
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"),FString("Hit React"));
	
	//Resistance
	GameplayTags.Attribute_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resistance.Fire"),FString(""));
	GameplayTags.Attribute_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resistance.Lightning"),FString(""));
	GameplayTags.Attribute_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resistance.Arcane"),FString(""));
	GameplayTags.Attribute_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resistance.Physical"),FString(""));
	
	//DamageType
	GameplayTags.DamageTag_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageTag.Fire"),FString("Fire Damage Type"));
	GameplayTags.DamageTag_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageTag.Lightning"),FString("Lightning Damage Type"));
	GameplayTags.DamageTag_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageTag.Lightning"),FString("Lightning Damage Type"));
	GameplayTags.DamageTag_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageTag.Physical"),FString("Physical Damage Type"));
	
	GameplayTags.DamageTypesTOResistances.Add(GameplayTags.DamageTag_Fire,GameplayTags.Attribute_Resistance_Fire);
	GameplayTags.DamageTypesTOResistances.Add(GameplayTags.DamageTag_Lightning,GameplayTags.Attribute_Resistance_Lightning);
	GameplayTags.DamageTypesTOResistances.Add(GameplayTags.DamageTag_Arcane,GameplayTags.Attribute_Resistance_Arcane);
	GameplayTags.DamageTypesTOResistances.Add(GameplayTags.DamageTag_Physical,GameplayTags.Attribute_Resistance_Physical);
}