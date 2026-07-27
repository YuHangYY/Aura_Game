// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AuraSummonAbility.h"


TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector ForWard = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector RightOfSpread = ForWard.RotateAngleAxis(SpawnSpread / 2.f,FVector::UpVector);
	
	const float DeltaSpread = SpawnSpread / NumMinions;//90/5;
	TArray<FVector> Result;
	for (int32 i = 0 ; i < NumMinions ; i++)
	{
		const FVector Direction = RightOfSpread.RotateAngleAxis(-DeltaSpread * i,FVector::UpVector);
		 FVector ChoseSpawnLocation = Location + Direction * FMath::RandRange(MinSpawnDistance,MaxSpawnDistance);
		
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit,ChoseSpawnLocation + FVector(0.f,0.f,400),ChoseSpawnLocation - FVector(0.f,0.f,400.f),ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			ChoseSpawnLocation = Hit.ImpactPoint;
		}
		Result.Add(ChoseSpawnLocation);
	} 
	
	return Result;

}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	int32 SelectRandom = FMath::RandRange(0,MinionClass.Num()-1);
	return MinionClass[SelectRandom];
}
