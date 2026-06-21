// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraAbilityInfo.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct FAbilityInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<const UMaterialInstance> BackgroundMaterial = nullptr;
};


UCLASS()
class AURA_GAME_API UAuraAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
	

public:
	
	FAbilityInfo FindAbilityInfoByTag(const FGameplayTag& AbilityTag,bool bLogNotFound= false);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Ability Information")
	TArray<FAbilityInfo> AbilityInformation;
	
	
	
};
