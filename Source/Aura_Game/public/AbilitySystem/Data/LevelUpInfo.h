// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FLeveLInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePoint = 1;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int32 SpellPoint = 1;
};


UCLASS()
class AURA_GAME_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Experience Information")
	TArray<FLeveLInfo> LeveLInformation;
	
	int32 FindLevelForXP(int32 XP)const;
};
