// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "CombatInterface.generated.h"
class UAbilitySystemComponent;
enum class ECharacterClass : uint8;
class UNiagaraSystem;
struct FGameplayTag;
class UAnimMontage;
// This class does not need to be modified.
DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered,UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath,AActor*,DeathActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathDelegate,AActor*,DeathActor);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageSignature,float/*伤害数值*/)


USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* AttackMontage; 
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag MontageTag;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag SocketTag;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
};

UINTERFACE(MinimalAPI,BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_GAME_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual FOnASCRegistered GetOnASCRegisteredDelegate() = 0;
	virtual FOnDeath GetOnDeathDelegate() = 0;
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	USkeletalMeshComponent* GetWeaponComponent();
	
	UFUNCTION(BlueprintNativeEvent)
	 int32 GetPlayerLevel();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	 FVector GetCombaWeaponLocation(const FGameplayTag& MontageTag);
	
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UAnimMontage* GetHitAnimMontage();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	TArray<FTaggedMontage> GetAttackAnimMontage();
	
	virtual void Die() = 0;
	
	virtual FOnDeathDelegate& GetOnDeathDelegate2() = 0;
	virtual FOnDamageSignature& GetOnDamageDelegate() = 0;
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool IsDead()const;
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	AActor* GetAvatar();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	FTaggedMontage GetTaggedMontage(const FGameplayTag& MontageTag);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	int32 GetMinionCount();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void SetMinionCount(int32 NewCount);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	ECharacterClass GetCharacterClassByClass();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)	
	void SetIsShockLoop(bool IsShockLoop);
};
