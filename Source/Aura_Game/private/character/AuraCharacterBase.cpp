


#include "character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "UAuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Aura_Game/Aura_Game.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


AAuraCharacterBase::AAuraCharacterBase()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	
	DebuffNiagaraComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffComponent");
	DebuffNiagaraComponent->SetupAttachment(GetRootComponent());
	DebuffNiagaraComponent->DebuffTag = FUAuraGameplayTags::Get().Debuff_Burn;
	
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Overlap);
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
    Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	CharacterClass = ECharacterClass::Elementalist;
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::Die()
{
	//死亡则掉落武器
	Weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	OnMulticastClientDeath();
}

void AAuraCharacterBase::DisSolve()
{
	if (IsValid(DisSolveMaterial))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DisSolveMaterial,this);
		GetMesh()->SetMaterial(0,DynamicMatInst);
		StartActorDisSolveTimeLine(DynamicMatInst);
	}
	
	if (IsValid(WeaponDisSolveMaterial))
	{
		UMaterialInstanceDynamic* WeaponMatInst = UMaterialInstanceDynamic::Create(WeaponDisSolveMaterial,this);
		Weapon->SetMaterial(0,WeaponMatInst);
		StartWeaponDisSolveTimeLine(WeaponMatInst);
	}
}

void AAuraCharacterBase::OnMulticastClientDeath_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this,DeathSound,GetActorLocation());
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	DisSolve();
	bIsDead = true;
	
	OnDeath.Broadcast(this);
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void AAuraCharacterBase::InitAbilityActorInfo()
{
	
}


FVector AAuraCharacterBase::GetCombaWeaponLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FUAuraGameplayTags& TagContain = FUAuraGameplayTags::Get();
	if (MontageTag.MatchesTagExact(TagContain.CombatSocket_Weapon)&&IsValid(Weapon))
	{
		return  Weapon->GetSocketLocation(SocketWeaponName);
	}
	
	if (MontageTag.MatchesTagExact(TagContain.CombatSocket_LeftHand))
	{
		return  GetMesh()->GetSocketLocation(SocketLeftName);
	}
	
	if (MontageTag.MatchesTagExact(TagContain.CombatSocket_RightHand))
	{
		return  GetMesh()->GetSocketLocation(SocketRightName);
	}
	if (MontageTag.MatchesTagExact(TagContain.CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(SocketTailName);
	}
	return FVector();
}

UAnimMontage* AAuraCharacterBase::GetHitAnimMontage_Implementation()
{
	return HitReactMontage;
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bIsDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage>  AAuraCharacterBase::GetAttackAnimMontage_Implementation()
{
	return AttackMontage;
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage AAuraCharacterBase::GetTaggedMontage_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontage)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 AAuraCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void AAuraCharacterBase::SetMinionCount_Implementation(int32 NewCount)
{
	MinionCount+=NewCount;
}

ECharacterClass AAuraCharacterBase::GetCharacterClassByClass_Implementation()
{
	return CharacterClass;
}

FOnASCRegistered AAuraCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnAscRegistered;
}

FOnDeath AAuraCharacterBase::GetOnDeathDelegate()
{
	return OnDeath;
}


void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level)const
{
	check(AbilitySystemComponent);
	check(EffectClass);
	 FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClass,Level,EffectContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitializeDefaultAttribute()const
{
	ApplyEffectToSelf(DefaultPrimaryAttribute,1.f);
	ApplyEffectToSelf(DefaultSecondaryAttribute,1.f);
	ApplyEffectToSelf(DefaultVitalAttribute,1.f);
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	UAuraAbilitySystemComponent* ASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;
	
	ASC->AddCharacterAbilities(StartUpAbilities);
	ASC->AddCharacterPassiveAbilities(PassiveAbility);
	
	
}






