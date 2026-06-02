/*
*  此cPP封装所有游戏的角色中的东西，比如武器，组件，属性集
 */

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_GAME_API AAuraCharacterBase : public ACharacter ,public  IAbilitySystemInterface,public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	//获取技能系统组件
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//获取属性集
	UAttributeSet* GetAttributeSet() const{return AttributeSet;}
	
	
	
	UFUNCTION(NetMulticast,Reliable)
	void OnMulticastClientDeath();
	
	void DisSolve();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartActorDisSolveTimeLine(UMaterialInstanceDynamic* MaterialInstanceDynamic);
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDisSolveTimeLine(UMaterialInstanceDynamic* MaterialInstanceDynamic);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DisSolveMaterial;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDisSolveMaterial;
protected:
	
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	
	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	FName SocketWeaponName;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	FName SocketLeftName;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	FName SocketRightName;
	
	bool bIsDead = false;
	
	virtual void Die() override; 
	virtual FVector GetCombaWeaponLocation_Implementation(const FGameplayTag& MontageTag)override;
	virtual UAnimMontage*GetHitAnimMontage_Implementation()override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackAnimMontage_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation()  override ;
	
	//技能系统组件
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attribute")
    TSubclassOf<UGameplayEffect> DefaultPrimaryAttribute;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attribute")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttribute;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attribute")
	TSubclassOf<UGameplayEffect> DefaultVitalAttribute;
	
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass,float Level)const;
	
	virtual void InitializeDefaultAttribute()const;
	
	void AddCharacterAbilities();
	
	UPROPERTY(EditAnywhere,Category="Combat")
	UNiagaraSystem* BloodEffect;
	
private:
	UPROPERTY(EditAnywhere,Category="Ability")
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	TArray<FTaggedMontage> AttackMontage;
	
};
