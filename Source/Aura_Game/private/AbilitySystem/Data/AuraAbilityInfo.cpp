// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AuraAbilityInfo.h"

#include "Aura_Game/AuraLogChannels.h"

FAbilityInfo UAuraAbilityInfo::FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound)
{
	for (const FAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogAura,Error,TEXT("Can't find info for AbilityInfo"));
	}
	
	return FAbilityInfo();
}
