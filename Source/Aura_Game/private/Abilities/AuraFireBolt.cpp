// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AuraFireBolt.h"
#include "UAuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Library/AuraWidgetControllerLibrary.h"


FString UAuraFireBolt::GetDescriptionCurrent(int32 Level)
{
	const float ManaCost = GetManaCost(Level); 
	float ScDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT("火焰箭\n发射对目标造成伤害，并且有几率使对方燃烧。火焰箭当前等级%d,发射%d个，伤害造成%f点火焰伤害.\n火焰箭消耗%f法力值,冷却为%f秒"),Level,FMath::Min(Level,MumFireBolt),ScDamage,FMath::Abs(ManaCost),Cooldown);
}

FString UAuraFireBolt::GetNextDescription(int32 Level)
{
	const float ManaCost = GetManaCost(Level); 
	float ScDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT("火焰箭\n发射对目标造成伤害，并且有几率使对方燃烧。火焰箭当前等级%d,发射%d个，伤害造成%f点火焰伤害.\n火焰箭消耗%f法力值,冷却为%f秒"),Level,FMath::Min(Level,MumFireBolt),ScDamage,FMath::Abs(ManaCost),Cooldown);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,bool IsOverPitch, float OverridePitch, AActor* HomingTarget)
{
	const bool IsServer =GetActorInfo().AvatarActor.Get()->HasAuthority() ;//要在服务器上调用
	if (!IsServer) return;
	
	//设置当前生成位置等属性
	FVector SocketLocation =  ICombatInterface::Execute_GetCombaWeaponLocation(GetAvatarActorFromActorInfo(),SocketTag);
	FTransform SpawnTransform;
	FRotator Rotation =(ProjectileTargetLocation - SocketLocation).Rotation();
	if (IsOverPitch) Rotation.Pitch = OverridePitch;
	
	
	FVector Forward = Rotation.Vector();
	int32 EffectNumProjectile = FMath::Min(GetAbilityLevel(),NumProjectiles);
	
	TArray<FRotator> Rotators = UAuraWidgetControllerLibrary::EvenlySpacedRotators(Forward,FVector::UpVector,ProjectileSpread,EffectNumProjectile);
	
	for (const FRotator& Rotator : Rotators)
	{
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			SpawnClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			//如果命中的是实现了战斗接口，那么设置目标为目标根组件
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation); 
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMagnitudeMin,HomingAccelerationMagnitudeMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = true;
	
		Projectile->FinishSpawning(SpawnTransform);
	}
	
	
	
}
