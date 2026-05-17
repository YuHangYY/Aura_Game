// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"


//属性访问器函数，通过辅助函数来进行返回属性，值，设置和初始化方法
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

struct FUAuraGameplayTags;

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()
	
public:
	
	
	UPROPERTY()
	UAbilitySystemComponent* SourceASC;
	UPROPERTY()
	AActor * SourceAvatarActor;
	UPROPERTY()
	APlayerController* SourcePC;
	UPROPERTY()
	ACharacter* SourceAvatarCharacter;
	
	FGameplayEffectContextHandle SourceEffectContextHandle;
	
	UPROPERTY()
	UAbilitySystemComponent* TargetASC;
	UPROPERTY()
	AActor * TargetAvatarActor;
	UPROPERTY()
	APlayerController* TargetPC;
	UPROPERTY()
	ACharacter* TargetAvatarCharacter;
	
	FGameplayEffectContextHandle TargetEffectContextHandle;
};

template<class T>
using TStaFuncPtr =   TBaseStaticDelegateInstance<T,FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class AURA_GAME_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAuraAttributeSet();
	//实现属性复制的关键一步
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	/*简单限制数据范围*/
	//某个 Attribute 真正被改掉之前，给你一次拦截和修改新值的机会。!!!!!!做数值限制（Clamp）
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	
	TMap<FGameplayTag,TStaFuncPtr<FGameplayAttribute()>> TagToAttributeMapping;
	
	/*次要属性*/
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_Armor,Category="属性集的属性")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Armor);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_ArmorPenetration,Category="属性集的属性")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,ArmorPenetration);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_BlockChance,Category="属性集的属性")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,BlockChance);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_CriticalHitChance,Category="属性集的属性")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CriticalHitChance);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_CriticalHitDamage,Category="属性集的属性")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CriticalHitDamage);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_CriticalHitResistance,Category="属性集的属性")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CriticalHitResistance);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_HealthRegeneration,Category="属性集的属性")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,HealthRegeneration);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_ManaRegeneration,Category="属性集的属性")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,ManaRegeneration);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_FireResistance,Category="Damage Resitance")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,FireResistance);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_LightningResistance,Category="Damage Resitance")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,LightningResistance);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_ArcaneResistance,Category="Damage Resitance")
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,ArcaneResistance);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_PhysicalResistance,Category="Damage Resitance")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,PhysicalResistance);
	
	//主要属性
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_Strength,Category="属性集的属性")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Strength);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_Intelligence,Category="属性集的属性")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Intelligence);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_Resilience,Category="属性集的属性")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Resilience);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_Vigor,Category="属性集的属性")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Vigor);
	
	/*Vital Property*/
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_Health,Category="属性集的属性")
	FGameplayAttributeData Health;//生命
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Health);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_MaxHealth,Category="属性集的属性")
	FGameplayAttributeData MaxHealth;//最大生命
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxHealth);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_Mana,Category="属性集的属性")
	FGameplayAttributeData Mana;//法力
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Mana);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_MaxMana,Category="属性集的属性")
	FGameplayAttributeData MaxMana;//最大法力
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxMana);
	/*end Vital Property*/
	
	
	//Meta Attributes//   元属性：不复制，在服务器端计算，计算最后独立的中介
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_MaxMana,Category="Meta Attributes")
	FGameplayAttributeData IncomingDamage;//伤害计算
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,IncomingDamage);
	
	
	
	/*start 函数*/
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth)const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& MaxOldHealth)const;
	
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& MaxOldMana)const;
	
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength)const;
	
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)const;
	
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience)const;
	
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor)const;
	
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor)const;
	
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration)const;
	
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance)const;
	
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance)const;
	
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage)const;
	
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance)const;
	
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration)const;
	
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration)const;
	
	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance)const;
	
	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance)const;
	
	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OldLightningResistance)const;
	
	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance)const;
	
	/*end 函数*/
	
private:
	void SetEffectProperties(const struct FGameplayEffectModCallbackData& Data,FEffectProperties& Props)const;
};
