// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_GAME_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
public:
	//返回小怪生成位置
	UFUNCTION(BlueprintCallable,Category="Summoning")
	TArray<FVector> GetSpawnLocations();
	
	UFUNCTION(BlueprintPure,Category="Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();
	
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	int32 NumMinions = 5; //最大生成数量
	
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float MinSpawnDistance = 50.0f; //最小距离
	
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float MaxSpawnDistance = 250.0f;//最大距离
	
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float SpawnSpread = 90.0f;//生成角度
	
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	TArray<TSubclassOf<APawn>> MinionClass;
};
