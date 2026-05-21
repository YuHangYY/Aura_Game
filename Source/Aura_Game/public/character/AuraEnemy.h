
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Widget/OverlapWidgetController.h"
#include "AuraEnemy.generated.h"

class AAuraAIController;
class UBehaviorTree;
enum class ECharacterClass : uint8;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_GAME_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();
	
	virtual void PossessedBy(AController* NewController) override;
	//接口实现函数
	virtual  void HighlightActor() override;
	virtual  void UnHighlightActor() override;
	//end
	
	virtual int32 GetPlayerLevel() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSign OnHealthChange;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSign OnMaxHealthChange;
	
	UPROPERTY(BlueprintReadOnly,Category="Combat")
	bool bIsHitReact = false;
	
	UPROPERTY(BlueprintReadOnly,Category="Combat")
	float BaseWalkSpeed = 250.f;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Combat")
	float LifeSpan = 5.f;
	
	virtual void Die() override;
protected:
	virtual  void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttribute() const override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	UPROPERTY(EditAnywhere,Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY()
    TObjectPtr<AAuraAIController> AuraAIController;
	
	
	//黑板键
private:
	
	UFUNCTION()
	void OnHitReactChange(const FGameplayTag Tag, int32 NewCount);
	
	UPROPERTY(EditAnywhere,Category="Character class Default")
	int32 Level=1;
	
	UPROPERTY(EditAnywhere,Category="Character class Default")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	
	
};
