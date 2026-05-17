// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameMode.generated.h"


class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class AURA_GAME_API AAuraGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly,Category="CharacterClassInfo")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
};
