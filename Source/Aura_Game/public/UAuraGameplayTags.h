// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 单例
 */

struct FUAuraGameplayTags
{
public:
	static const FUAuraGameplayTags& Get(){return GameplayTags;}
	static void InitializeNativeGameplayTags();
	
	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Vigor;
	
	//Secondary
	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_ArmorPenetration;
	FGameplayTag Attribute_Secondary_BlockChance;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_CriticalHitResistance;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;
	
	FGameplayTag Attribute_Resistance_Fire;
	FGameplayTag Attribute_Resistance_Lightning;
	FGameplayTag Attribute_Resistance_Arcane;
	FGameplayTag Attribute_Resistance_Physical;
	
	//input
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;
	
	//Abilities
	FGameplayTag Ability_Attack;
	FGameplayTag Ability_Summon;
	FGameplayTag Ability_None;
	
	
	FGameplayTag Ability_HitReact;
	
	FGameplayTag Ability_Status_Locked;
	FGameplayTag Ability_Status_Eligible;
	FGameplayTag Ability_Status_Unlocked;
	FGameplayTag Ability_Status_Equipped;
	
	FGameplayTag Ability_Type_Passive;
	FGameplayTag Ability_Type_Offensive;
	FGameplayTag Ability_Type_None;
	
	FGameplayTag Ability_Fire_FireBolt;
	FGameplayTag Ability_Lightning_Electrocute;
	
	FGameplayTag Cooldown_Fire_FireBolt;
	
	//Effect
	FGameplayTag Effects_HitReact;
	
	//DamageTag
	FGameplayTag Damage;
	FGameplayTag DamageTag_Fire;//火元素伤害
	FGameplayTag DamageTag_Lightning;//雷电伤害
	FGameplayTag DamageTag_Arcane;//奥术伤害
	FGameplayTag DamageTag_Physical;//物理伤害
	
	//Debuff
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Physical;
	
	
	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Frequency;
	FGameplayTag Debuff_Duration;
	
	//CombatSocket
	FGameplayTag CombatSocket_Weapon; 
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;
	
	//Montage
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	
	//Meta
	FGameplayTag Attribute_Meta_IncomingXP;
	
	TMap<FGameplayTag,FGameplayTag> DamageTypesTOResistances;
	TMap<FGameplayTag,FGameplayTag> DamageTypesTODebuffs;
protected:
	
private:
	static FUAuraGameplayTags GameplayTags; 
};
