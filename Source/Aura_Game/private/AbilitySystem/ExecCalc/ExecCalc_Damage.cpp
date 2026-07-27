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
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	
	TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition> TagToCaptureDef;
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitChance,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitDamage,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,FireResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,LightningResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArcaneResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,PhysicalResistance,Target,false);
		
		
		
		
		
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics Statics; //全局实例 
	const FUAuraGameplayTags& Tags = FUAuraGameplayTags::Get(); 
	Statics.TagToCaptureDef.Add(Tags.Attribute_Secondary_Armor,Statics.ArmorDef);
	Statics.TagToCaptureDef.Add(Tags.Attribute_Secondary_ArmorPenetration,Statics.ArmorPenetrationDef);
	Statics.TagToCaptureDef.Add(Tags.Attribute_Secondary_BlockChance,Statics.BlockChanceDef);
	Statics.TagToCaptureDef.Add(Tags.Attribute_Secondary_CriticalHitChance,Statics.CriticalHitChanceDef);
	Statics.TagToCaptureDef.Add(Tags.Attribute_Secondary_CriticalHitDamage,Statics.CriticalHitDamageDef);
	Statics.TagToCaptureDef.Add(Tags.Attribute_Secondary_CriticalHitResistance,Statics.CriticalHitResistanceDef);
	Statics.TagToCaptureDef.Add(Tags.Attribute_Resistance_Fire,Statics.FireResistanceDef);
	Statics.TagToCaptureDef.Add(Tags.Attribute_Resistance_Lightning,Statics.LightningResistanceDef);
	Statics.TagToCaptureDef.Add(Tags.Attribute_Resistance_Arcane,Statics.ArcaneResistanceDef);
	Statics.TagToCaptureDef.Add(Tags.Attribute_Resistance_Physical,Statics.PhysicalResistanceDef);
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
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	 UAbilitySystemComponent* SourceAsc= ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetAsc = ExecutionParams.GetTargetAbilitySystemComponent();
	
	AActor* SourActor = SourceAsc->GetAvatarActor();
	AActor* TargetActor = TargetAsc->GetAvatarActor();
	
	int32 SourcePlayerLevel = 1;
	if (SourActor->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourActor);
	}
	
	int32 TargetPlayerLevel = 1;
	if (TargetActor->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetActor);
	}
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters AggregatorEvaluatorParams;
	AggregatorEvaluatorParams.SourceTags = SourceTag;
	AggregatorEvaluatorParams.TargetTags = TargetTag;
	
	float Damage = 0.f;	
	for (TPair<FGameplayTag, FGameplayTag> Pair : FUAuraGameplayTags::Get().DamageTypesTOResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTypeTag = Pair.Value;
		
		check(DamageStatics().TagToCaptureDef.Contains(ResistanceTypeTag));
		const FGameplayEffectAttributeCaptureDefinition ResistanceDef = DamageStatics().TagToCaptureDef[ResistanceTypeTag];
		
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag);
		
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ResistanceDef,AggregatorEvaluatorParams,Resistance);
		Resistance = FMath::Clamp(Resistance,0.f,100.f);
		
		DamageTypeValue *= (100.f - Resistance)/100.f;
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
	const float ArmorPenetrationvalue = ArmorPenetrationCurve->Eval(SourcePlayerLevel);
	
	const float EffectiveArmor = TargetArmor *=(100 - SourceArmorPenetration* ArmorPenetrationvalue) / 100.f;
	
	FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationTable->FindCurve(FName("EffectiveArmor"),FString());
	const float EffectiveArmorValue = EffectiveArmorCurve->Eval(TargetPlayerLevel);
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
	const float CriticalHitResistanceValue =  CriticalHitResistanceCurve->Eval(TargetPlayerLevel);
	
	float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceValue;
	const bool IsCriticalHit = FMath::RandRange(1,100) < EffectiveCriticalHitChance;
	
	UAuraWidgetControllerLibrary::SetCriticalHit(EffectContextHandle,IsCriticalHit);
	
	Damage = IsCriticalHit ? Damage*2.f + SourceCriticalHitDamage : Damage; 
	
	FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
}
