// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/AuraWidgetControllerLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityType.h"
#include "Game/AuraGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "UI/Widget/AuraWidgetController.h"

UOverlapWidgetController* UAuraWidgetControllerLibrary::GetOverlapWidgetController(UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams Params(PC,PS,ASC,AS);
			return HUD->GetOverlapWidgetController(Params);
		}
	}
	return nullptr;
	
}

UAttributeMenuWidgetController* UAuraWidgetControllerLibrary::GetAttributeMenuWidgetController(UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			 UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams Params(PC,PS,ASC,AS);
			return HUD->GetAttributeMenuController(Params);
		}
	}
	return nullptr;
}

void UAuraWidgetControllerLibrary::InitializeCharacterClassInfo(const UObject* WorldContextObject,ECharacterClass CharacterClass, float Level,
	UAbilitySystemComponent* ASC)
{
	AAuraGameMode* Mode =Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (Mode==nullptr) return;
	
	//从mode获取资产，然后调用根据职业获取对应的效果
	UCharacterClassInfo* CharacterClassInfo  = Mode->CharacterClassInfo;
	const FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(ASC->GetAvatarActor());
	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassDefaultInfo.PrimaryAttributes,Level,EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
	
	FGameplayEffectContextHandle SecondEffectContextHandle = ASC->MakeEffectContext();
	SecondEffectContextHandle.AddSourceObject(ASC->GetAvatarActor());
	const FGameplayEffectSpecHandle SecondEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondsAttributes,Level,SecondEffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondEffectSpecHandle.Data.Get());
	
	
	FGameplayEffectContextHandle VitalEffectContextHandle = ASC->MakeEffectContext();
	VitalEffectContextHandle.AddSourceObject(ASC->GetAvatarActor());
	const FGameplayEffectSpecHandle VitalEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes,Level,VitalEffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalEffectSpecHandle.Data.Get());
	
}

void UAuraWidgetControllerLibrary::GiveEnemyStartUpAbilities(const UObject* WorldContextObject,UAbilitySystemComponent* ASC)
{
	AAuraGameMode* Mode =Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (Mode==nullptr) return;
	
	for (const TSubclassOf<UGameplayAbility>& CharacterClassInfo : Mode->CharacterClassInfo->StartUpAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CharacterClassInfo,1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UAuraWidgetControllerLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameMode* Mode =Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (Mode==nullptr) return nullptr;
	
	return  Mode->CharacterClassInfo;
}

bool UAuraWidgetControllerLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->IsBlockHit();
	}
	return false;
}

bool UAuraWidgetControllerLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->IsCriticalHit();
	}
	return false;
}

void UAuraWidgetControllerLibrary::SetBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bBlockedHit)
{
	if ( FAuraGameplayEffectContext* AuraContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetBlockHit(bBlockedHit);
	}
}

void UAuraWidgetControllerLibrary::SetCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bCriticalHit)
{
	if ( FAuraGameplayEffectContext* AuraContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetCriticalHit(bCriticalHit);
	}
}
