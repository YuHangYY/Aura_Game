// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_GAME_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input")
	FGameplayTag StartupInputTag;
	
	virtual FString GetDescriptionCurrent(int32 Level);
	virtual FString GetNextDescription(int32 Level);
	static FString GetLockedDescriptionCurrent();
	
	float GetManaCost(int32 Level = 1.f);
	float GetCooldown(int32 Level = 1.f);
};
