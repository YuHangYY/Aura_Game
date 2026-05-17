// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_GAME_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();
	virtual int32 GetPlayerLevel() override;
	//在服务器上，当controller控制Pawn时，调用回调函数，进行组件初始化
	virtual void PossessedBy(AController* NewController) override;
	
	//在客户端上，同步数据
	virtual void OnRep_PlayerState() override;
	
	//封装函数用于初始化组件和属性集
	virtual void InitAbilityActorInfo() override;
};
