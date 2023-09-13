// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AvengerDebugWidgetItem.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Delegates/DelegateCombinations.h"
#include "Ui/Common/AvengerActivatableWidget.h"
#include "Ui/Common/AvengerButtonBase.h"
#include "Ui/Common/AvengerTabListWidgetBase.h"
#include "AvengerDebugWidget.generated.h"

class UDebugMenuEntry;

// DeactivateされてもTick処理をするため
UCLASS()
class UDebugTicker : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

	UDebugTicker() { }

public:
	virtual void Tick(float DeltaTime) override
	{
		if(!DebugCallback.IsValid())
			return;
		
		if(LastFrameNumberWeTicked == GFrameCounter)
			return;

		if(DebugCallback.Func != nullptr)
			DebugCallback.Func(0);

		DebugCallback.DynamicDelegate.ExecuteIfBound(0);
		
		LastFrameNumberWeTicked = GFrameCounter;
	}
	virtual ETickableTickType GetTickableTickType() const override
	{
		return ETickableTickType::Always;
	}
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FDebugTicker, STATGROUP_Tickables);
	}
	virtual bool IsTickableWhenPaused() const override
	{
		return true;
	}
	virtual bool IsTickableInEditor() const override
	{
		return false;
	}
	void SetCallback(FDebugCallback callback) { DebugCallback = callback; }

private:
	uint32 LastFrameNumberWeTicked = INDEX_NONE;
	FDebugCallback DebugCallback;
};

/**
 * 
 */
UCLASS(meta = (Category = "DebugMenu", DisableNativeTick))
class AVENGER_CLIENT_API UAvengerDebugWidget : public UAvengerActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void NativeOnActivated() override;

	UFUNCTION()
	void ClearDebugMenu();
	
	UFUNCTION()
	void OnChangeHoverMenu(UCommonButtonBase* target);

	UFUNCTION()
	void RequestCreateDynamicTab();
	
public:
	void AddDebugMenu(const TArray<UDebugMenuEntry*>& debugMenuEntry);

	UFUNCTION(BlueprintCallable)
	void NavigateMenu(FString tabID);

	UFUNCTION()
	void HandleApplyAction();

private:
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle ApplyInputActionData;
	
	FUIActionBindingHandle ApplyHandle;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = Input, meta = (BindWidget, OptionalWidget = true, AllowPrivateAccess = true))
	TObjectPtr<UAvengerTabListWidgetBase> TopDebugMenuTabs;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UHorizontalBox> MenuBox;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UAvengerButtonBase> TopMenuTabClass;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonButtonBase> ItemMenuClass;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonButtonBase> EnumItemMenuClass;

	UPROPERTY(BlueprintReadOnly, Transient)
	TMap<FString, UVerticalBox*> MenuMap;

	UPROPERTY(BlueprintReadOnly, Transient)
	UVerticalBox* CurrentMenu;

	UPROPERTY(Transient)
	TWeakObjectPtr<UCommonButtonBase> CurrentFocusMenu;
	
};