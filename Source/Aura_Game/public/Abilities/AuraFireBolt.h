// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_GAME_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescriptionCurrent(int32 Level);
	virtual FString GetNextDescription(int32 Level);
	
	UFUNCTION(BlueprintCallable,Category="FireBolt")
	void SpawnProjectiles(const FVector& ProjectileTargetLocation,const FGameplayTag& SocketTag,bool IsOverPitch,float OverridePitch,AActor* HomingTarget);
	
protected:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="FireBolt")
	float ProjectileSpread = 90.f;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="FireBolt")
	int32 MaxProjectile = 5;
	
	int32 MumFireBolt = 5; 
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="FireBolt")
	float HomingAccelerationMagnitudeMin = 1600.f;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="FireBolt")
	float HomingAccelerationMagnitudeMax = 3200.f;
};
