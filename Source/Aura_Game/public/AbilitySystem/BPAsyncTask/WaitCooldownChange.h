// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"
struct FGameplayEffectSpec;
struct FActiveGameplayEffectHandle;
struct FGameplayTag;
class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeDelegate, float, CooldownTime);
/**
 * 
 */
UCLASS(BlueprintType,meta=(ExposedAsyncProxy = "AsyncTask"))
class AURA_GAME_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeDelegate CooldownStart;
	
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeDelegate CooldownEnd;
	 
	UFUNCTION(BlueprintCallable,meta=(BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,const FGameplayTag& InCooldownTag); 
	
	UFUNCTION(BlueprintCallable)
	void EndTask();	
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	FGameplayTag CooldownTag;
	
	
	void CooldownTagChange(const FGameplayTag InCooldownTag,int32 NewCount);
	void OnActiveEffectAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
