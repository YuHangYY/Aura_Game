// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class AURA_GAME_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	UDebuffNiagaraComponent();
	
	UPROPERTY(EditAnywhere)
	FGameplayTag DebuffTag;
	
protected:
	virtual void BeginPlay() override;
	void DebuffTagChanced(const FGameplayTag Tag,int32 Count);
	
	UFUNCTION()
	void OwnDeath(AActor* Actor);
};
