// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/AuraDamageGameplayAbility.h"
#include "AuraBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class AURA_GAME_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);
	
	UFUNCTION(BlueprintCallable)
	void StoreOwingPlayerController();
	
	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);
	
	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(TArray<AActor*>& OutTargetActors);
	
	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDied(AActor* DeathActor);
	
	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDied(AActor* DeathActor);
protected:
	UPROPERTY(BlueprintReadWrite,Category="Beam")
	FVector MouseHitLocation;
	
	UPROPERTY(BlueprintReadWrite,Category="Beam")
	AActor* HitActor;
	
	UPROPERTY(BlueprintReadWrite,Category="Beam")
	TObjectPtr<APlayerController> OwningPlayerController;
	
	UPROPERTY(BlueprintReadWrite,Category="Beam")
	TObjectPtr<ACharacter> OwnerCharacter;
	
	UPROPERTY(EditDefaultsOnly,Category="Beam")
	int32 MaxNumShockTargets = 5;
};
