// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	const bool ISLocalController = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (ISLocalController)
	{
		//本地客户端：有鼠标，有输入，可以自己获取鼠标命中
		SendMouseCursorData();
	}
	else
	{
		// 服务器端：没有客户端鼠标，只能等客户端通过 RPC 发 TargetData
		
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle,GetActivationPredictionKey()).AddUObject(this,&UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		
		//情况二：客户端 TargetData 先到，服务器后绑定
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle,ActivationPredictionKey);
		//true :则调用了该TargetData 已经到了回调已经被调用
		//false : TargetData 还没到,服务器需要继续等待客户端数据
	    if (!bCalledDelegate)
	    {
		   SetWaitingOnRemotePlayerData(); 
	    }
	}
	
	
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	//预测窗口是告诉GAS系统该范围所有的操作都要被预测
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());
	
	APlayerController* PC =  Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorResult;
	PC->GetHitResultUnderCursor(ECC_Visibility,false,CursorResult);
	
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult =  CursorResult;
	DataHandle.Add(Data);
	
	//服务器RPC 将目标数据复制到服务器
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);
	
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		DataValid.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		DataValid.Broadcast(DataHandle);
	}
}
