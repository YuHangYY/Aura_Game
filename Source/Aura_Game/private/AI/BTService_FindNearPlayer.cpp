// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindNearPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	APawn* Owing = AIOwner->GetPawn();
	
	const FName TargetName= Owing->ActorHasTag(FName("Player"))? FName("Enemy"):FName("Player");
	
	TArray<AActor*> ActorsWithTags;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(),TargetName,ActorsWithTags);
	
	float DistanceToTarget = TNumericLimits<float>::Max();
	AActor* closetTarget = nullptr;
	
	for (AActor* Actor : ActorsWithTags)
	{
		const float LocalDistance = Owing->GetDistanceTo(Actor);
		if (LocalDistance < DistanceToTarget )
		{
			DistanceToTarget = LocalDistance;
			closetTarget = Actor; 
		}
	}
	
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this,DistanceToTargetSelector,DistanceToTarget);
	UBTFunctionLibrary::SetBlackboardValueAsObject(this,TargetToFollowSelector,closetTarget);
}
