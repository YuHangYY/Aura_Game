// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_GAME_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template<class UserClass,typename PressFuncType,typename ReleaseFuncType,typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig,UserClass* Object,PressFuncType PressFunc,ReleaseFuncType ReleaseFunc,HeldFuncType HeldFunc);
};

template <class UserClass, typename PressFuncType, typename ReleaseFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object ,PressFuncType PressFunc,ReleaseFuncType ReleaseFunc, HeldFuncType HeldFunc)
{
	//获取你写的AbilityinputActions数组里的数据，然后通过输入绑定对应的函数，把tag作为参数传出去
	check(InputConfig);
	
	for (const FAuraInputAction& config :InputConfig->AbilityInputActions)
	{
		if (config.InputAction&&config.InputTag.IsValid())
		{
			if (PressFunc)
			{
				BindAction(config.InputAction,ETriggerEvent::Started,Object,PressFunc,config.InputTag);
			}
			
			if (ReleaseFunc)
			{
				BindAction(config.InputAction,ETriggerEvent::Completed,Object,ReleaseFunc,config.InputTag);
			}
			
			if (HeldFunc)
			{
				BindAction(config.InputAction,ETriggerEvent::Triggered,Object,HeldFunc,config.InputTag);
			}
		}
	}
}
