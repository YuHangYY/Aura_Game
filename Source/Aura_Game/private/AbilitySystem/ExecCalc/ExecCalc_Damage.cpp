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

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters AggregatorEvaluatorParams,const TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition>& TagToCaptureDef) const
{
	FUAuraGameplayTags GameplayTags = FUAuraGameplayTags::Get();
	for (TPair<FGameplayTag, FGameplayTag>& Pair : GameplayTags.DamageTypesTODebuffs )
	{
		//1.判断是什么伤害，然后获取对应的debuff标签
		FGameplayTag DamageTag = Pair.Key;
		FGameplayTag DebuffTag = Pair.Value;
		float TypeDamage = Spec.GetSetByCallerMagnitude(DamageTag,false,-1.f);
		if (TypeDamage > 0.5f)
		{
			//如果大于-1.就代表这个是正确的值，对应的debuff效果也是这个
			float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance,false,-1.f);
			float TargetDebuffResistance = 0.f;
			const FGameplayTag& Resistance = GameplayTags.DamageTypesTOResistances[DamageTag];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagToCaptureDef[Resistance],AggregatorEvaluatorParams,TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max(TargetDebuffResistance,0);
			const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance) /100.0f;
			const bool bDebuff = FMath::RandRange(1,100) < EffectiveDebuffChance;
			if (bDebuff)
			{
				//通过EffectContextHandle,来传递信息
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
				UAuraWidgetControllerLibrary::SetSuccessfulDebuff(ContextHandle,true);
				UAuraWidgetControllerLibrary::SetDamageType(ContextHandle,DamageTag);
				
				float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage,false,-1.f);
				float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration,false,-1.f);
				float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency,false,-1.f);
				
				UAuraWidgetControllerLibrary::SetDebuffDuration(ContextHandle,DebuffDuration);
				UAuraWidgetControllerLibrary::SetDebuffFrequency(ContextHandle,DebuffFrequency);
				UAuraWidgetControllerLibrary::SetDebuffDamage(ContextHandle,DebuffDamage);
			}
		}
		
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition> TagToCaptureDef;
	const FUAuraGameplayTags& Tags = FUAuraGameplayTags::Get(); 
	TagToCaptureDef.Add(Tags.Attribute_Secondary_Armor,DamageStatics().ArmorDef);
	TagToCaptureDef.Add(Tags.Attribute_Secondary_ArmorPenetration,DamageStatics().ArmorPenetrationDef);
	TagToCaptureDef.Add(Tags.Attribute_Secondary_BlockChance,DamageStatics().BlockChanceDef);
	TagToCaptureDef.Add(Tags.Attribute_Secondary_CriticalHitChance,DamageStatics().CriticalHitChanceDef);
	TagToCaptureDef.Add(Tags.Attribute_Secondary_CriticalHitDamage,DamageStatics().CriticalHitDamageDef);
	TagToCaptureDef.Add(Tags.Attribute_Secondary_CriticalHitResistance,DamageStatics().CriticalHitResistanceDef);
	TagToCaptureDef.Add(Tags.Attribute_Resistance_Fire,DamageStatics().FireResistanceDef);
	TagToCaptureDef.Add(Tags.Attribute_Resistance_Lightning,DamageStatics().LightningResistanceDef);
	TagToCaptureDef.Add(Tags.Attribute_Resistance_Arcane,DamageStatics().ArcaneResistanceDef);
	TagToCaptureDef.Add(Tags.Attribute_Resistance_Physical,DamageStatics().PhysicalResistanceDef);
	
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
	
	
	//Debuff处理
	DetermineDebuff(ExecutionParams, Spec, AggregatorEvaluatorParams,TagToCaptureDef);
	//Debuff处理
	
	float Damage = 0.f;	
	for (TPair<FGameplayTag, FGameplayTag> Pair : FUAuraGameplayTags::Get().DamageTypesTOResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTypeTag = Pair.Value;
		
		check(TagToCaptureDef.Contains(ResistanceTypeTag));
		const FGameplayEffectAttributeCaptureDefinition ResistanceDef = TagToCaptureDef[ResistanceTypeTag];
		
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
