// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AuraBeamSpell.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

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
				EDrawDebugTrace::ForDuration,HitResult,
				true);
			
			if (HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				HitActor = HitResult.GetActor();
			}
		}
		
	}
	
}
