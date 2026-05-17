// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraWidgetControllerLibrary.generated.h"

struct FGameplayEffectContextHandle;
class UCharacterClassInfo;
class UAbilitySystemComponent;
enum class ECharacterClass : uint8;
class UAttributeMenuWidgetController;
class UOverlapWidgetController;
/**
 * 
 */
UCLASS()
class AURA_GAME_API UAuraWidgetControllerLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Aura")
	static UOverlapWidgetController* GetOverlapWidgetController(UObject* WorldContextObject); 
	
	UFUNCTION(BlueprintPure, Category = "Aura")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable,Category="Aura")
	static void InitializeCharacterClassInfo(const UObject* WorldContextObject,ECharacterClass CharacterClass,float Level,UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable,Category="Aura")
	static void GiveEnemyStartUpAbilities(const UObject* WorldContextObject,UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable,Category="Aura")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category="Effect")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	
	UFUNCTION(BlueprintCallable, Category="Effect")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	static void SetBlockedHit(FGameplayEffectContextHandle& EffectContextHandle,bool bBlockedHit);
	
	UFUNCTION(BlueprintCallable, Category="Effect")
	static void SetCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,bool bCriticalHit);
	
};
