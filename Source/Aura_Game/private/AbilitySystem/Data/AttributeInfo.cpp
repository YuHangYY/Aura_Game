// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& GameplayTag, bool IsFound ) const
{
	for (const FAuraAttributeInfo& AttributeInfo : AttributeInformation )
	{
		if (AttributeInfo.AttributeTag == GameplayTag)
		{
			IsFound = true;
			return AttributeInfo;
		}
	}
	return FAuraAttributeInfo();
}
