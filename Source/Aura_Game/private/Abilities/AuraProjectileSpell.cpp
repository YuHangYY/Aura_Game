// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "UAuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "character/AuraCharacter.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation,const FGameplayTag& SocketTag,bool IsOverPitch,float OverridePitch)
{
	const bool IsServer =GetActorInfo().AvatarActor.Get()->HasAuthority() ;//要在服务器上调用
	
	if (!IsServer) return;
	
		//设置当前生成位置等属性
		FVector SocketLocation =  ICombatInterface::Execute_GetCombaWeaponLocation(GetAvatarActorFromActorInfo(),SocketTag);
		FTransform SpawnTransform;
		FRotator Rotation =(ProjectileTargetLocation - SocketLocation).Rotation();
		if (IsOverPitch)
		{
			Rotation.Pitch = OverridePitch;
		}
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			SpawnClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		const UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(DamageEffect,ICombatInterface::Execute_GetPlayerLevel(GetAvatarActorFromActorInfo()),EffectContext);
		
		for (auto& Pair : DamageTypes)
		{
			float ScalarFloat = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle,Pair.Key,ScalarFloat);
		}
		
		Projectile->DamageEffectSpecHandle = EffectSpecHandle;
	
		
		Projectile->FinishSpawning(SpawnTransform);
	
}

