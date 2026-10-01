/*
*  此cPP封装所有游戏的角色中的东西，比如武器，组件，属性集
 */

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UPassiveNiagaraComponent;
class UDebuffNiagaraComponent;
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
	
	//对该游戏实体造成伤害
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Tick(float DeltaTime) override;
	AAuraCharacterBase();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
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
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	USoundBase* DeathSound;
	
	FOnASCRegistered OnAscRegistered;
	FOnDeath OnDeath;
	FOnDeathDelegate OnDeathDelegate;
	FOnDamageSignature OnDamageDelegate;
	
	UPROPERTY(Replicated,BlueprintReadOnly)
	bool IsStun = false;
	
	virtual void StunTagChance(const FGameplayTag CallTag,int32 NewCount);
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
	
	UPROPERTY(EditAnywhere,Category="Combat")
	FName SocketTailName;
	
	bool bIsDead = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	float BaseWalkSpeed = 600.f;
	
	virtual void Die() override; 
	virtual FOnDeathDelegate& GetOnDeathDelegate2() override;
	virtual USkeletalMeshComponent* GetWeaponComponent_Implementation() override;
	virtual FVector GetCombaWeaponLocation_Implementation(const FGameplayTag& MontageTag)override;
	virtual UAnimMontage*GetHitAnimMontage_Implementation()override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackAnimMontage_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation()  override ;
	virtual FTaggedMontage GetTaggedMontage_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void SetMinionCount_Implementation(int32 NewCount) override;
	virtual ECharacterClass GetCharacterClassByClass_Implementation() override;
	virtual FOnASCRegistered GetOnASCRegisteredDelegate() override;
	virtual FOnDeath GetOnDeathDelegate() override;
	virtual FOnDamageSignature& GetOnDamageDelegate() override;
	
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
	
	int32 MinionCount = 0;
	
	UPROPERTY(EditAnywhere,Category="Character class Default")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	
private:
	UPROPERTY(EditAnywhere,Category="Ability")
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;
	
	UPROPERTY(EditAnywhere,Category="Ability")
	TArray<TSubclassOf<UGameplayAbility>>  PassiveAbility;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	TArray<FTaggedMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDebuffNiagaraComponent> DebuffNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;
};
