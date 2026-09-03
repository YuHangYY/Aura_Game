// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilityType.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "UAuraGameplayTags.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Library/AuraWidgetControllerLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	const FUAuraGameplayTags& GameplayTags = FUAuraGameplayTags::Get();
	//primary
	TagToAttributeMapping.Add(GameplayTags.Attribute_Primary_Strength,GetStrengthAttribute);
	TagToAttributeMapping.Add(GameplayTags.Attribute_Primary_Intelligence,GetIntelligenceAttribute);
	TagToAttributeMapping.Add(GameplayTags.Attribute_Primary_Resilience,GetResilienceAttribute);
	TagToAttributeMapping.Add(GameplayTags.Attribute_Primary_Vigor,GetVigorAttribute);
	
	//Secondary
	
	TagToAttributeMapping.Add(GameplayTags.Attribute_Secondary_Armor,GetArmorAttribute);
	TagToAttributeMapping.Add(GameplayTags.Attribute_Secondary_ArmorPenetration,GetArmorPenetrationAttribute);
	TagToAttributeMapping.Add(GameplayTags.Attribute_Secondary_BlockChance,GetBlockChanceAttribute);
	TagToAttributeMapping.Add(GameplayTags.Attribute_Secondary_CriticalHitChance,GetCriticalHitChanceAttribute);
	TagToAttributeMapping.Add(GameplayTags.Attribute_Secondary_CriticalHitDamage,GetCriticalHitDamageAttribute);
	TagToAttributeMapping.Add(GameplayTags.Attribute_Secondary_CriticalHitResistance,GetCriticalHitResistanceAttribute);
	TagToAttributeMapping.Add(GameplayTags.Attribute_Secondary_HealthRegeneration,GetHealthRegenerationAttribute);
	TagToAttributeMapping.Add(GameplayTags.Attribute_Secondary_ManaRegeneration,GetManaRegenerationAttribute);
	TagToAttributeMapping.Add(GameplayTags.Attribute_Secondary_MaxHealth,GetMaxHealthAttribute);
	TagToAttributeMapping.Add(GameplayTags.Attribute_Secondary_MaxMana,GetMaxManaAttribute);
	TagToAttributeMapping.Add(GameplayTags.Attribute_Resistance_Fire,GetFireResistanceAttribute);
	TagToAttributeMapping.Add(GameplayTags.Attribute_Resistance_Lightning,GetLightningResistanceAttribute);
	TagToAttributeMapping.Add(GameplayTags.Attribute_Resistance_Arcane,GetArcaneResistanceAttribute);
	TagToAttributeMapping.Add(GameplayTags.Attribute_Resistance_Physical,GetPhysicalResistanceAttribute);
	
}

void UAuraAttributeSet::SetEffectProperties(const struct FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.SourceEffectContextHandle = Data.EffectSpec.GetEffectContext();
	Props.SourceASC = Props.SourceEffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo)
	{
		Props.SourceAvatarActor = Props.SourceASC->GetAvatarActor();
		Props.SourcePC = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceAvatarActor  != nullptr && Props.SourcePC == nullptr)
		{
			const APawn* SourceAvatarPawn = Cast<APawn>(Props.SourceAvatarActor);
			Props.SourcePC = Cast<APlayerController>(SourceAvatarPawn->GetController());
		}
		if (Props.SourcePC)
		{
			Props.SourceAvatarCharacter = Cast<ACharacter>(Props.SourcePC->GetPawn());
		}
	}
	if (IsValid(&Data.Target))
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetPC = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetAvatarCharacter =  Cast<ACharacter>(Props.TargetAvatarActor );
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor );
		
	}
}

void UAuraAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (Props.TargetAvatarActor->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Props.TargetAvatarActor);
		const ECharacterClass CharacterClass = ICombatInterface::Execute_GetCharacterClassByClass(Props.TargetAvatarActor);
		const int32 XPForward = UAuraWidgetControllerLibrary::GetXPRewardForClassAndLevel(Props.TargetAvatarActor,CharacterClass,TargetLevel);
		
		FUAuraGameplayTags GameplayTags = FUAuraGameplayTags::Get();
		
		FGameplayEventData Payload ;
		Payload.EventTag = GameplayTags.Attribute_Meta_IncomingXP;
		Payload.EventMagnitude = XPForward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceAvatarActor,GameplayTags.Attribute_Meta_IncomingXP,Payload);
	}
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0,GetMaxHealth());
	}
	
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0,GetMaxMana());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FEffectProperties Props;
	SetEffectProperties(Data,Props);
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(),0.f,GetMaxHealth()));
	}
	
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(),0.f,GetMaxMana()));
	}
	
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}
	
	if (Data.EvaluatedData.Attribute == GetInComingXPAttribute())
	{
		HandleIncomingXp(Props);
	
	}
}

void UAuraAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
		if (Props.SourceAvatarActor->Implements<UCombatInterface>())
		{
			if (ICombatInterface::Execute_IsDead(Props.SourceAvatarActor) )return;
		}
	
	if (Props.TargetAvatarActor->Implements<UCombatInterface>())
	{
		if (ICombatInterface::Execute_IsDead(Props.TargetAvatarActor) )return;
	}
	
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth,0.f,GetMaxHealth()));
			float health = GetHealth();
			bool is = GetHealth() < 20;
			
			bool bDeath = GetHealth() <= 0;
			if (bDeath)
			{
				//死亡
				if (ICombatInterface* DieActor = Cast<ICombatInterface>(Props.TargetAvatarActor))
				{
					DieActor->Die();
				}
				//死亡掉落经验发送事件通知被动技能
				SendXPEvent(Props);
			}
			else
			{
				//被击中
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FUAuraGameplayTags::Get().Effects_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
				
				
			}
			
			if (UAuraWidgetControllerLibrary::IsSuccessfulDebuff(Props.SourceEffectContextHandle))
			{
				Debuff(Props);
			}
			//显示伤害在目标身上
			if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.SourcePC))
			{
				const bool isBlock = UAuraWidgetControllerLibrary::IsBlockedHit(Props.SourceEffectContextHandle);
				const bool isCritical = UAuraWidgetControllerLibrary::IsCriticalHit(Props.SourceEffectContextHandle);
				PC->ShowDamageText(LocalIncomingDamage,Props.TargetAvatarCharacter,isBlock,isCritical);
				return;
			}
			
			if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.TargetPC))
			{
				const bool isBlock = UAuraWidgetControllerLibrary::IsBlockedHit(Props.SourceEffectContextHandle);
				const bool isCritical = UAuraWidgetControllerLibrary::IsCriticalHit(Props.SourceEffectContextHandle);
				PC->ShowDamageText(LocalIncomingDamage,Props.TargetAvatarCharacter,isBlock,isCritical);
			}
			
			
		}
	}

void UAuraAttributeSet::HandleIncomingXp(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetInComingXP();
	SetInComingXP(0.f);
		  
	if (Props.SourceAvatarActor->Implements<UPlayerInterface>() && Props.SourceAvatarActor->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceAvatarCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceAvatarCharacter);
		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceAvatarCharacter,CurrentXP + LocalIncomingXP);
			
		const int32 NumOfLevelUp = NewLevel - CurrentLevel;
		if (NumOfLevelUp > 0)
		{
			const int32 LocalAttributePoint = IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceAvatarCharacter,CurrentLevel);
			const int32 LocalSpellPoint = IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceAvatarCharacter,CurrentLevel);
				
			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceAvatarCharacter,NumOfLevelUp);
			IPlayerInterface::Execute_AddToAttributePoint(Props.SourceAvatarCharacter,LocalAttributePoint);
			IPlayerInterface::Execute_AddToSpellPoint(Props.SourceAvatarCharacter,LocalSpellPoint);
				
			bTopOffMana = true;
			bTopOffHealth = true;
				
		
			
			IPlayerInterface::Execute_LevelUp(Props.SourceAvatarCharacter);
		}
			
		IPlayerInterface::Execute_AddToXP(Props.SourceAvatarCharacter,LocalIncomingXP);
		
		SetMaxHealth(GetMaxHealth());
		SetMaxMana(GetMaxMana());
	}
}

void UAuraAttributeSet::Debuff(const FEffectProperties& Props)
{
	FUAuraGameplayTags GameplayTags = FUAuraGameplayTags::Get();
	//动态创建Effect
	FGameplayEffectContextHandle EffectContextHand = Props.SourceASC->MakeEffectContext();
	EffectContextHand.AddSourceObject(Props.SourceAvatarActor);
	
	FGameplayTag DamageType = UAuraWidgetControllerLibrary::GetDamageType(Props.SourceEffectContextHandle);
	float DebuffDamage = UAuraWidgetControllerLibrary::GetDebuffDamage(Props.SourceEffectContextHandle);
	float DebuffDuration = UAuraWidgetControllerLibrary::GetDebuffDuration(Props.SourceEffectContextHandle);
	float DebuffFrequency = UAuraWidgetControllerLibrary::GetDebuffFrequency(Props.SourceEffectContextHandle);
	
	//创建一个空的Effect
	FString DebuffName = FString::Printf(TEXT("DynamicDebuff%s"),*DamageType.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(),FName(DebuffName));
	
	//设置内部参数
	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
	
	UTargetTagsGameplayEffectComponent& TargetTags = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer Container;
	Container.Added.AddTag(GameplayTags.DamageTypesTODebuffs[DamageType]);
	Container.CombinedTags.AddTag(GameplayTags.DamageTypesTODebuffs[DamageType]);
	TargetTags.SetAndApplyTargetTagChanges(Container);
	
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;
	
	const int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& Modifier = Effect->Modifiers[Index];
	
	Modifier.ModifierMagnitude = FScalableFloat(DebuffDamage);
	Modifier.Attribute = GetIncomingDamageAttribute();
	Modifier.ModifierOp = EGameplayModOp::Additive;
	
	FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect,EffectContextHand);
	if (MutableSpec)
	{
		FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHand.Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
		AuraContext->SetDamageType(DebuffDamageType);	
		
		const FActiveGameplayEffectHandle Handle = Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
		
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	
	if (Attribute == GetMaxManaAttribute() &&bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
	
	if (Attribute == GetMaxHealthAttribute() &&bTopOffHealth)
	{
		SetHealth(NewValue);
		bTopOffHealth = false;
	}
}



void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	//预测修改值
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Health,OldHealth);
	
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& MaxOldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxHealth,MaxOldHealth);
} 

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Mana,OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& MaxOldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxMana,MaxOldMana);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Strength,OldStrength)
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Intelligence,OldIntelligence)
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Resilience,OldResilience)
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Vigor,OldVigor)
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Armor,OldArmor)
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,ArmorPenetration,OldArmorPenetration)
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,BlockChance,OldBlockChance)
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,CriticalHitChance,OldCriticalHitChance)
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,CriticalHitDamage,OldCriticalHitDamage)
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,CriticalHitResistance,OldCriticalHitResistance)
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,HealthRegeneration,OldHealthRegeneration)
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,ManaRegeneration,OldManaRegeneration)
}

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,LightningResistance,OldFireResistance)
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,LightningResistance,OldLightningResistance)
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,PhysicalResistance,OldLightningResistance)
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,PhysicalResistance,OldPhysicalResistance)
}


void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//注册属性复制
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Armor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,ArmorPenetration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,BlockChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,CriticalHitChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,CriticalHitDamage,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,CriticalHitResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,HealthRegeneration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,ManaRegeneration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,FireResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,LightningResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,ArcaneResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,PhysicalResistance,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Strength,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Intelligence,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Resilience,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Vigor,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
}



