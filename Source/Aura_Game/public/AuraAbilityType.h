#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityType.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()
	
	FDamageEffectParams(){};
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject>	WorldContextObject = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;
	
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();
	
	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float DebuffDamageDuration = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false; //本次是否启动伤害削减机制
	
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f; //伤害削减内半径
	
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f; // 伤害削减外半径
	
	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector; //径向伤害原点
};


USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	
public:
	bool IsCriticalHit()const{return bIsCriticalHit;}
	bool IsBlockHit()const{return bIsBlockHit;}
	bool IsSuccessfulDebuff()const{return bSuccessfulDebuff;}
	bool IsRadialDamage()const{return bIsRadialDamage;}
	
	void SetSuccessfulDebuff(bool isSuccessfulDebuff){bSuccessfulDebuff = isSuccessfulDebuff;}
	void SetCriticalHit(bool InIsCriticalHit){bIsCriticalHit = InIsCriticalHit; };
	void SetBlockHit(bool InIsBlockHit){bIsBlockHit = InIsBlockHit; };
	void SetDebuffDamage(float Damage){DebuffDamage = Damage; };
	void SetDebuffFrequency(float Frequency){DebuffFrequency = Frequency; };
	void SetDebuffDuration(float Duration){DebuffDuration = Duration; };
	void SetDamageType(TSharedPtr<FGameplayTag> Tag){DamageType = Tag; };
	void SetRadialDamage(bool IsRadialDamage){bIsRadialDamage = IsRadialDamage; };
	void SetRadialDamageInnerRadius(float RadialDamageInnerRadiu){RadialDamageInnerRadius = RadialDamageInnerRadiu; };
	void SetRadialDamageOuterRadius(float RadialDamageOuterRadiu){RadialDamageOuterRadius = RadialDamageOuterRadiu; };
	void SetRadialDamageOrigin(FVector Origin){RadialDamageOrigin = Origin; };
	
	float GetDebuffDamage()const{return DebuffDamage;}
	float GetDebuffFrequency()const{return DebuffFrequency;}
	float GetDebuffDuration()const{return DebuffDuration;}  
	float GetRadialDamageInnerRadius()const{return RadialDamageInnerRadius;}
	float GetRadialDamageOuterRadius()const {return RadialDamageOuterRadius;}
	FVector GetRadialDamageOrigin()const{return RadialDamageOrigin;}
	
	TSharedPtr<FGameplayTag> GetDamageType()const{return DamageType;}
	
	/** 返回用于序列化的实际结构体，子类必须重写此方法！* */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
	
	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const override
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	//网络序列化
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override ;
	
protected:
	
	UPROPERTY()
	bool bIsBlockHit = false;
	
	UPROPERTY()
	bool bIsCriticalHit  = false;
	
	UPROPERTY()
	bool bSuccessfulDebuff = false;
	
	UPROPERTY()
	float DebuffDamage = 0.f;
	
	UPROPERTY()
	float DebuffFrequency = 0.f;
	
	UPROPERTY()
	float DebuffDuration = 0.f;
	
	UPROPERTY()
	bool bIsRadialDamage = false; //本次是否启动伤害削减机制
	
	UPROPERTY()
	float RadialDamageInnerRadius = 0.f; //伤害削减内半径
	
	UPROPERTY()
	float RadialDamageOuterRadius = 0.f; // 伤害削减外半径
	
	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector; //径向伤害原点
	
	TSharedPtr<FGameplayTag> DamageType;
};

template<>
struct TStructOpsTypeTraits< FAuraGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FAuraGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
