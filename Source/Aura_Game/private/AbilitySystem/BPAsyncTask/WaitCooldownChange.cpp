// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BPAsyncTask/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,const FGameplayTag& InCooldownTag)
{
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;
	
	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}
	
	//冷却什么时候结束
	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag,EGameplayTagEventType::NewOrRemoved).AddUObject(
		WaitCooldownChange,
		&UWaitCooldownChange::CooldownTagChange
		);
	
	//冷却什么时候开始
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange,&UWaitCooldownChange::OnActiveEffectAdded);
	
	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (IsValid(ASC))
	{
		ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	}
	//准备删除和标记为垃圾
	SetReadyToDestroy();
	MarkAsGarbage();
	
}

void UWaitCooldownChange::CooldownTagChange(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0.f)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer AsstTags;
	EffectSpec.GetAllAssetTags(AsstTags); 
	
	FGameplayTagContainer GrantedTag;
	EffectSpec.GetAllGrantedTags(GrantedTag);
	
	if (AsstTags.HasTagExact(CooldownTag) || GrantedTag.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimeRemainingArray = ASC->GetActiveEffectsTimeRemaining(Query);
		
		if (TimeRemainingArray.Num() > 0)
		{
			float TimeRemaining = TimeRemainingArray[0];
			
			for (int i = 0;i<TimeRemainingArray.Num();i++)
			{
				if (TimeRemainingArray[i]>TimeRemaining)
				{
					TimeRemaining = TimeRemainingArray[i];
				}
			}
			
			CooldownStart.Broadcast(TimeRemaining);
		}
		
	}
	
}
