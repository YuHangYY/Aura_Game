// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityType.h"
#include "UAuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "kismet/KismetMathLibrary.h"
#include "Library/AuraWidgetControllerLibrary.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitChance,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitDamage,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitResistance,Target,false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics Statics; //全局实例 
	return Statics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	 UAbilitySystemComponent* SourceAsc= ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetAsc = ExecutionParams.GetTargetAbilitySystemComponent();
	
	AActor* SourActor = SourceAsc->GetAvatarActor();
	AActor* TargetActor = TargetAsc->GetAvatarActor();
	ICombatInterface* Source = Cast<ICombatInterface>(SourActor);
	ICombatInterface*Target = Cast<ICombatInterface>(TargetActor);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters AggregatorEvaluatorParams;
	AggregatorEvaluatorParams.SourceTags = SourceTag;
	AggregatorEvaluatorParams.TargetTags = TargetTag;
	
	float Damage = 0.f;	
	for (auto DamageTag : FUAuraGameplayTags::Get().DamageTypesTOResistances)
	{
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTag.Key);
		Damage += DamageTypeValue;
	}
	
	//格挡
	float Blockchange = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,AggregatorEvaluatorParams,Blockchange);
	Blockchange = FMath::Max(Blockchange ,0.f);
	
	bool isBlock =UKismetMathLibrary::RandomFloatInRange(1,100)<=Blockchange;
	Damage = isBlock? Damage*=0.5f: Damage;
	
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UAuraWidgetControllerLibrary::SetBlockedHit(EffectContextHandle,isBlock);
	
	//护甲
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,AggregatorEvaluatorParams,TargetArmor);
	TargetArmor = FMath::Max(TargetArmor ,0.f);
	
	//自身护甲穿透
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,AggregatorEvaluatorParams,SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(SourceArmorPenetration ,0.f);
	
	UCharacterClassInfo* CharacterClassInfo = UAuraWidgetControllerLibrary::GetCharacterClassInfo(SourActor);
	
	FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationTable->FindCurve(FName("ArmorPenetration"),FString());
	const float ArmorPenetrationvalue = ArmorPenetrationCurve->Eval(Source->GetPlayerLevel());
	
	const float EffectiveArmor = TargetArmor *=(100 - SourceArmorPenetration* ArmorPenetrationvalue) / 100.f;
	
	FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationTable->FindCurve(FName("EffectiveArmor"),FString());
	const float EffectiveArmorValue = EffectiveArmorCurve->Eval(Target->GetPlayerLevel());
	Damage *= (100 - EffectiveArmor * EffectiveArmorValue) / 100.f; 
	
	//暴击率
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,AggregatorEvaluatorParams,SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max(SourceCriticalHitChance ,0.f);
	//暴击抗性
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef,AggregatorEvaluatorParams,TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max(TargetCriticalHitResistance ,0.f);
	//暴击加成
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef,AggregatorEvaluatorParams,SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max(SourceCriticalHitDamage ,0.f);
	
	FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationTable->FindCurve(FName("CriticalHitResistance"),FString());
	const float CriticalHitResistanceValue =  CriticalHitResistanceCurve->Eval(Target->GetPlayerLevel());
	
	float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceValue;
	const bool IsCriticalHit = FMath::RandRange(1,100) < EffectiveCriticalHitChance;
	
	UAuraWidgetControllerLibrary::SetCriticalHit(EffectContextHandle,IsCriticalHit);
	
	Damage = IsCriticalHit ? Damage*2.f + SourceCriticalHitDamage : Damage; 
	
	FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
}
