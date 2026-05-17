// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UDamageTextComponent;
class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class IEnemyInterface;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
	

/**
 * 
 */
UCLASS()
class AURA_GAME_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	
	UFUNCTION(Client,Reliable)
	void ShowDamageText(float Damage,ACharacter* Chara,bool IsBlock,bool IsCritical);
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	
private:
	//输入映射上下文
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> ShiftAction;
	
	void ShiftPressed(){IsShiftDown = true;}
	void ShiftReleased(){IsShiftDown = false;}
	bool IsShiftDown = false;
	
	void Move(const  FInputActionValue& Value);
	void AbilityInputPress(FGameplayTag InputTag);
	void AbilityInputRelease(FGameplayTag InputTag);
	void AbilityInputHeld(FGameplayTag InputTag);
	UAuraAbilitySystemComponent* GetASC();
	
	
	void AutoMoveing();
	void CursorTrace();
	IEnemyInterface* LastAction ;
	IEnemyInterface* ThisAction ;
	FHitResult Hit;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAuraInputConfig> InputConfig;
	
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;
	
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool AutoRunning =false;
	bool bIsCursorTracing = false;
	
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageWidgetComponent;
};
