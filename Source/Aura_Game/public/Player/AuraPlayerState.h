// Fill out your copyright notice in the Description page of Project Settings.
/*
 * 此玩家状态，将属性和数据部署在这里是为了防止角色死亡之后数据全部归零，然后保存数据，当角色死亡数据不会丢失，全局只有一个玩家状态
 */
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class AURA_GAME_API AAuraPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AAuraPlayerState();
	FORCEINLINE int32 GetPlayerLevel()const{return Level;}
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	//获取Aura的技能系统组件
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//获取Aura的属性集
	UAttributeSet* GetAttributeSet() const{return AttributeSet;}
protected:
	//技能系统组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
private:
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_Level)
	int32 Level = 1;
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
};
