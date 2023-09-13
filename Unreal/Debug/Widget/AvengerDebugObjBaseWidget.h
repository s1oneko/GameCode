// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AvengerDebugObjListInterface.h"
#include "Blueprint/UserWidget.h"
#include "Delegates/DelegateCombinations.h"
#include "AvengerDebugObjBaseWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSwitchingObjWidget, const UObject*, objectRef);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnClosingTabWidget, const class UAvengerDebugObjListTabWidget*, widgetRef);
DECLARE_DYNAMIC_DELEGATE(FOnClosingPropertyWidget);

UENUM(BlueprintType)
enum class EDebugObjListSortType : uint8
{
	None = 0,
	MemUsage = 1,
	ObjCount = 2,
	Alphanumerical = 3
};

/**
 * 
 */
UCLASS()
class AVENGER_CLIENT_API UAvengerDebugObjBaseWidget : public UUserWidget, public IAvengerDebugObjListInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void OnPressSearchButton(const FString searchText);
	
	UFUNCTION(BlueprintCallable)
	void OnPressRefreshButton(EDebugObjListSortType objListSortType = EDebugObjListSortType::MemUsage);

	UFUNCTION(BlueprintCallable)
	void OnPressSearchIndexButton(int value);
	
	UFUNCTION()
	void OnClosedTabWidget(class UAvengerDebugObjListTabWidget* widgetRef);

	UFUNCTION()
	void OnClosedPropertyWidget();

	UFUNCTION()
	void OnSwitchingTab(UObject* objectRef);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UAvengerDebugObjListWidget> ObjListWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UAvengerDebugObjPropertyWidget> ObjPropertyWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UAvengerDebugObjListTabWidget> ObjTabWidgetClass;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UHorizontalBox> TabHorizontalBox;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(Transient)
	TWeakObjectPtr<UAvengerDebugObjListWidget> ObjListWidget;

	UPROPERTY(Transient)
	TWeakObjectPtr<UAvengerDebugObjPropertyWidget> ObjPropertyWidget;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<UAvengerDebugObjListTabWidget>> ObjListTabWidgets;

	FOnSwitchingObjWidget OnSwitchingObjWidget;
	FOnClosingPropertyWidget OnClosingPropertyWidget;
	FOnClosingTabWidget OnClosingTabWidget;
};