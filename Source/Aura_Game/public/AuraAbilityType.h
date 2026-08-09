#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityType.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()
	
	FDamageEffectParams(){};
	
	UPROPERTY()
	TObjectPtr<UObject>	WorldContextObject = nullptr;
	
	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;
	
	UPROPERTY()
	float BaseDamage = 0.f;
	
	UPROPERTY()
	float AbilityLevel = 1.f;
	
	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();
	
	UPROPERTY()
	float DebuffChance = 0.f;
	
	UPROPERTY()
	float DebuffDamage = 0.f;
	
	UPROPERTY()
	float DebuffFrequency = 0.f;
	
	UPROPERTY()
	float DebuffDamageDuration = 0.f;
};


USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	
public:
	bool IsCriticalHit()const{return bIsCriticalHit;}
	bool IsBlockHit()const{return bIsBlockHit;}
	bool IsSuccessfulDebuff()const{return bSuccessfulDebuff;}
	
	void SetSuccessfulDebuff(bool isSuccessfulDebuff){bSuccessfulDebuff = isSuccessfulDebuff;}
	void SetCriticalHit(bool InIsCriticalHit){bIsCriticalHit = InIsCriticalHit; };
	void SetBlockHit(bool InIsBlockHit){bIsBlockHit = InIsBlockHit; };
	void SetDebuffDamage(float Damage){DebuffDamage = Damage; };
	void SetDebuffFrequency(float Frequency){DebuffFrequency = Frequency; };
	void SetDebuffDuration(float Duration){DebuffDuration = Duration; };
	void SetDamageType(TSharedPtr<FGameplayTag> Tag){DamageType = Tag; };
	
	float GetDebuffDamage()const{return DebuffDamage;}
	float GetDebuffFrequency()const{return DebuffFrequency;}
	float GetDebuffDuration()const{return DebuffDuration;}  
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
