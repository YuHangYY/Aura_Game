// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/AuraWidgetControllerLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityType.h"
#include "Game/AuraGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "UI/Widget/AuraWidgetController.h"
#include "Widgets/SDMXReadOnlyFixturePatchList.h"

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

void UAuraWidgetControllerLibrary::GiveEnemyStartUpAbilities(const UObject* WorldContextObject,UAbilitySystemComponent* ASC,ECharacterClass CharacterClass)
{
	AAuraGameMode* Mode =Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (Mode==nullptr) return;
	
	for (const TSubclassOf<UGameplayAbility>& CharacterClassInfo : Mode->CharacterClassInfo->StartUpAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CharacterClassInfo,1);
		ASC->GiveAbility(AbilitySpec);
	}
	
	FCharacterClassDefaultInfo DefaultInfo = Mode->CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
	{
		for (TSubclassOf<UGameplayAbility> Ability : DefaultInfo.DedicatedAbilities)
		{
		
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability,ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(Ability);
		}
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

void UAuraWidgetControllerLibrary::GetLifeActorWithingRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutActors, const TArray<AActor*> OtherActors, float Radius, FVector SphereLocation)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(OtherActors);

	if (const UWorld*World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereLocation, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		
		for (FOverlapResult &Overlap : Overlaps)
		{
			bool ImplementCombatInterface = Overlap.GetActor()->Implements<UCombatInterface>();
			if (ImplementCombatInterface )
			{
				bool IsAlive = !ICombatInterface::Execute_IsDead(Overlap.GetActor());
			
				if (IsAlive)
				{
					OutActors.AddUnique(Overlap.GetActor());
				}
			}
			
		}
	}
}

bool UAuraWidgetControllerLibrary::ISBothFirend(AActor* FirstActor, AActor* SecondActor)
{
	const bool ISBothPlayer = FirstActor->ActorHasTag(FName("Player"))&&SecondActor->ActorHasTag(FName("Player"));
	const bool IsBothEnemy = FirstActor->ActorHasTag(FName("Enemy"))&&SecondActor->ActorHasTag(FName("Enemy"));
	
	const bool IsFriend = IsBothEnemy || ISBothPlayer;
	
	return !IsFriend;
}

int32 UAuraWidgetControllerLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,ECharacterClass CharacterClass, int32 Level)
{
	 UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo==nullptr) return 0.f;
	
	const FCharacterClassDefaultInfo& ClassInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	float XP = ClassInfo.XPReward.GetValueAtLevel(Level);
	
	return static_cast<int32>(XP);
}
