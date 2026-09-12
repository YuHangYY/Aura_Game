// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

struct FDamageEffectParams;
struct FTaggedMontage;
/**
 * 
 */
UCLASS()
class AURA_GAME_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* Target);
	
	UFUNCTION(BlueprintCallable)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* Target =nullptr)const ;
	
	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel()const;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<UGameplayEffect> DamageEffect;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Damage")
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Damage")
	FScalableFloat Damage;
	
	/*Debuff*/
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Debuff")
	float DebuffChance = 20.f; 
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Debuff")
	float DebuffDamage = 5.f; 
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Debuff")
	float DebuffFrequency = 1.f; 
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Debuff")
	float DebuffDuration = 5.f; 
	
	/*Debuff*/
	
	
	UFUNCTION(BlueprintCallable)
	FTaggedMontage GetRandomAttackMontageFromArray(const TArray<FTaggedMontage>& MontageArray);
	
};
