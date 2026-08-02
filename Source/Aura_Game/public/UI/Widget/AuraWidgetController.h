// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.generated.h"

class UAuraAttributeSet;
class UAuraAbilitySystemComponent;
class AAuraPlayerState;
class AAuraPlayerController;
class UAuraAbilityInfo;
class UAttributeSet;
class UAbilitySystemComponent;
class APlayerState;
class APlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangeSign,int32,Newvalue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSign,const FAbilityInfo& ,ABilityInfo);
//蓝图类型结构体方便初始化
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()
public:
	FWidgetControllerParams(){}
	FWidgetControllerParams(APlayerController* PC,APlayerState* PS,UAbilitySystemComponent* Asc,UAttributeSet* As)
	:PlayerController(PC),PlayerState(PS),AbilitySystemComponent(Asc),AttributeSet(As){}
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet;
};
/**
 * 
 */
UCLASS()
class AURA_GAME_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
public:
	//初始化控制器参数
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& Params);
	
	//广播给widget的值
	
	virtual void BroadcastInitialValues();
	virtual void BindCallbackToDependencies();
	
	void BroadcastAbilityInfo();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Widget Data")
	TObjectPtr<UAuraAbilityInfo> AbilityInfo; 
protected:
	UPROPERTY(BlueprintAssignable,Category="GAS|Delegate")
	FAbilityInfoSign AbilityInfoDelegate;
	
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;
	
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<AAuraPlayerController> AuraPlayerController;
	
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<AAuraPlayerState> AuraPlayerState;
	
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;
	
	
	AAuraPlayerController* GetAuraPC();
	AAuraPlayerState* GetAuraPlayerState();
	UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent();
	UAuraAttributeSet* GetAuraAttributeSet();

	
};
