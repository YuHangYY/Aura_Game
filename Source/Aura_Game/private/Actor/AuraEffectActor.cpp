
#include "Actor/AuraEffectActor.h"

#include "ActiveGameplayEffectHandle.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneComponent"));
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor,TSubclassOf<UGameplayEffect> GamePlayEffectClass)
{
	
	const bool IsEnemy = TargetActor->ActorHasTag("Enemy");
	if (IsEnemy && !bApplyEffectToEnemy) return;
	
	UAbilitySystemComponent* ASCTraget =UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);  
	
	if (!ASCTraget) return;
	
	check(GamePlayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = ASCTraget->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = ASCTraget->MakeOutgoingSpec(GamePlayEffectClass,ActorLevel,EffectContextHandle);
	FActiveGameplayEffectHandle ActiveEffectHandle = ASCTraget->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
	//获取spec的Duration的类型，如果该GE是无限的并且是离开时停止的，加到我们map中，等到离开的时候删除map里的数据
	 const bool IsInfinite =  EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (IsInfinite && InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle,ASCTraget);
	}
	
	if (bDestroyOnEffectRemove && EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Instant||EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::HasDuration)
	{
		Destroy();
	}
	
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	const bool IsEnemy = TargetActor->ActorHasTag("Enemy");
	if (IsEnemy && !bApplyEffectToEnemy) return;
	
	
	if (InstantEffectApplicationPolicy ==EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGamePlayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,DurationGamePlayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,InfiniteGamePlayEffectClass);
	}
}

void AAuraEffectActor::EndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGamePlayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,DurationGamePlayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InfiniteGamePlayEffectClass);
	}
	if (InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* ASCTraget = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!ASCTraget) return;
		
		TArray<FActiveGameplayEffectHandle> Handles;
		for ( TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*>& handle:ActiveEffectHandles)
		{
			if (handle.Value == ASCTraget)
			{
				ASCTraget->RemoveActiveGameplayEffect(handle.Key,1);
				Handles.Add(handle.Key);
			}
		}
		for (FActiveGameplayEffectHandle& handle : Handles)
		{
			ActiveEffectHandles.FindAndRemoveChecked(handle);
		}
		
	}
}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	
}




