// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AuraBeamSpell.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Library/AuraWidgetControllerLibrary.h"

void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		HitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true);
	}
	
}

void UAuraBeamSpell::StoreOwingPlayerController()
{
	if (CurrentActorInfo)
	{
		OwningPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon =ICombatInterface::Execute_GetWeaponComponent(OwnerCharacter))
		{
			
			TArray<AActor*> ActorsToIgnore;
			FHitResult HitResult;
			ActorsToIgnore.Add(OwnerCharacter);
			const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));
			
			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter,
				SocketLocation,
				BeamTargetLocation,
				10.f,
				TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::ForDuration,
				HitResult,
				true);
			
			if (HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				HitActor = HitResult.GetActor();
				
				
			}
		}
		
	}
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(HitActor))
	{
		if (!CombatInterface->GetOnDeathDelegate2().IsAlreadyBound(this,&UAuraBeamSpell::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathDelegate2().AddDynamic(this,&UAuraBeamSpell::PrimaryTargetDied);
		}
					
	}
}

void UAuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutTargetActors)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(HitActor);
	
	TArray<AActor*> OverlappingActors;
	UAuraWidgetControllerLibrary::GetLifeActorWithingRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		850,
		HitActor->GetActorLocation());
	
	//int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1,MaxNumShockTargets);
	int32 NumAdditionalTargets = 5;
	
	UAuraWidgetControllerLibrary::GetClosestTargets(NumAdditionalTargets,OverlappingActors,OutTargetActors,HitActor->GetActorLocation());
	
	for (AActor* Actor : OutTargetActors)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Actor))
		{
			if (!CombatInterface->GetOnDeathDelegate2().IsAlreadyBound(this,&UAuraBeamSpell::AdditionalTargetDied))
			{
				CombatInterface->GetOnDeathDelegate2().AddDynamic(this,&UAuraBeamSpell::AdditionalTargetDied);
			}
					
		}
	}
	
}
