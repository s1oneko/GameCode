// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonRotator.h"
#include "Debug/AvengerDebugWidgetInterface.h"
#include "AvengerDebugWidgetEnumItem.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class AVENGER_CLIENT_API UAvengerDebugWidgetEnumItem : public UCommonRotator, public IAvengerDebugWidgetInterface
{
	GENERATED_BODY()

private:
	UFUNCTION(BlueprintCallable)
	UDebugMenuSubsystem* GetDebugMenuSubsystem();
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnUnhovered() override;

public:
	// WidgetInterface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnApplyAction();
	virtual void OnApplyAction_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnChangeValue(int32 values);
	virtual void OnChangeValue_Implementation(int32 values) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetDebugMenuEntryData(UDebugMenuEntry* debugMenuEntry);
	virtual void SetDebugMenuEntryData_Implementation(UDebugMenuEntry* entryData) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnCallDelegate();
	virtual void OnCallDelegate_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSaveDebugValue();
	virtual void OnSaveDebugValue_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnChangeNavigate();
	virtual void OnChangeNavigate_Implementation() override;

	virtual FOnChangeHover* GetOnChangeHoverDelegate() override
	{
		return &OnChangeHover;
	}
	// WidgetInterface End

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<const UDebugMenuEntry> DebugMenuEntry;
	
protected:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Init;
	
	UPROPERTY(BlueprintCallable)
	FOnChangeHover OnChangeHover;

	UPROPERTY(BlueprintReadOnly)
	uint8 DebugValue;

	UPROPERTY(Transient)
	TObjectPtr<class UDebugTicker> DebugTicker;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> SoundOnUnhover;
};
