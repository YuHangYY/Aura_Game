// Fill out your copyright notice in the Description page of Project Settings.


#include "character/AuraCharacter.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "UAuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
	
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::LevelUp_Implementation()
{
	Client_LevelUpNiagaraActive_Implementation();
}

void AAuraCharacter::Client_LevelUpNiagaraActive_Implementation()
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		LevelUpNiagaraComponent->Activate(true);
	}
	
}

int32 AAuraCharacter::GetXP_Implementation()
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetExperience();
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 CurLevel) const
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LeveLInfoPtr->LeveLInformation[CurLevel].AttributePoint;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 CurLevel) const
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LeveLInfoPtr->LeveLInformation[CurLevel].SpellPoint;
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 NewLevel)
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(NewLevel);
	
	if (UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		ASC->UpdateAbilityStatus(AuraPlayerState->GetPlayerLevel());
	}
}

void AAuraCharacter::AddToAttributePoint_Implementation(int32 NewAttributePoint)
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToAttributePoints(NewAttributePoint);
}

void AAuraCharacter::AddToSpellPoint_Implementation(int32 NewSpellPoint)
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToSpellPoints(NewSpellPoint);
}

void AAuraCharacter::AddToXP_Implementation(int32 XP)
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	AuraPlayerState->AddToXP(XP);
}

int32 AAuraCharacter::FindLevelForXP_Implementation(int32 XP)
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	return AuraPlayerState->LeveLInfoPtr->FindLevelForXP(XP);
}

int32 AAuraCharacter::GetAttributePoint_Implementation()
{
	AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSpellPoint_Implementation()
{
	AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetSpellPoints();
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	//在服务器端上将技能添加到组件管理
	AddCharacterAbilities();
	
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState * AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	////初始化组件的拥有者和扮演者，拥有者代表逻辑上，扮演者代表世界里的
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	OnAscRegistered.Broadcast(AbilitySystemComponent);
	
	if (AAuraPlayerController * AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			
			AuraHUD->InitOverlap(AuraPlayerController,AuraPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}
	if (IsValid(AbilitySystemComponent))
	{ 
		AbilitySystemComponent->RegisterGameplayTagEvent(FUAuraGameplayTags::Get().Debuff_Stun,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&AAuraCharacter::StunTagChance);
	}
	InitializeDefaultAttribute();
	
}


