// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/Widget/AuraWidgetController.h"
#include "OverlapWidgetController.generated.h"

class UAuraUserWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow:public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText MessageText = FText();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D *Image = nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;
};


struct FOnAttributeChangeData;
//动态多播委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeSign,float,Newvalue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSign,FUIWidgetRow,Row);


/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class AURA_GAME_API UOverlapWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbackToDependencies() override;
	
	
	
	
	/*多播委托的变量*/
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangeSign HealthChangedSign;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangeSign MaxHealthChangedSign;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangeSign ManaChangedSign;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangeSign MaxManaChangedSign;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Message")
	FMessageWidgetRowSign MessageWidgetRowDelegate;
	/*多播委托的变量*/
protected:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="WidgetMessage")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	template<class T>
	T* GetDataTableRowByTag(UDataTable* Table,FGameplayTag& Tag);
};

template <class T>
T* UOverlapWidgetController::GetDataTableRowByTag(UDataTable* Table, FGameplayTag& Tag)
{
	T* Row = Table->FindRow<T>(Tag.GetTagName(),TEXT(""));
	return Row;
}
