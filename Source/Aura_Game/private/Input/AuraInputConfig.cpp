// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindInputActionByTag(const FGameplayTag& Tag) const
{
	for (const FAuraInputAction& Data :AbilityInputActions)
	{	
		if (Data.InputAction&& Data.InputTag == Tag)
		{
			return Data.InputAction;
		}
		
	}
	return nullptr; 
}
