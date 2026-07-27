// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntDef.bSnapshot = false;
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const		FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluatedParameters;
	EvaluatedParameters.SourceTags= SourceTags;
	EvaluatedParameters.TargetTags= TargetTags;
	
	float Resilience = 0.f;
	GetCapturedAttributeMagnitude(IntDef, Spec,EvaluatedParameters,Resilience);
	Resilience = FMath::Max(Resilience,0.f);
	
	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}
	
	
	
	return 50.f+2.5*Resilience+10.f*PlayerLevel; 
}
