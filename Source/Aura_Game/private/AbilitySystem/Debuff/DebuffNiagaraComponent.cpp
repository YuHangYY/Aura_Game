// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (ASC)
	{
		ASC->RegisterGameplayTagEvent(DebuffTag,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&UDebuffNiagaraComponent::DebuffTagChanced);
	}
	else if (CombatInterface)
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this,[this](UAbilitySystemComponent* InASC)
		{
			InASC->RegisterGameplayTagEvent(DebuffTag,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&UDebuffNiagaraComponent::DebuffTagChanced);
		}
		);
	}
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this,&UDebuffNiagaraComponent::OwnDeath);
	}
}

void UDebuffNiagaraComponent::DebuffTagChanced(const FGameplayTag Tag, int32 Count)
{
	if (Count > 0)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UDebuffNiagaraComponent::OwnDeath(AActor* Actor)
{
	Deactivate();
}
