// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AvengerDebugHelper.h"
#include "CommonButtonBase.h"
#include "UObject/Interface.h"
#include "AvengerDebugWidgetInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishDebugEntry, UDebugMenuEntry*, debugMenuEntry);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeHover, UCommonButtonBase*, target);
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAvengerDebugWidgetInterface : public UInterface
{
	GENERATED_BODY()

};

/**
 * 
 */
class AVENGER_CLIENT_API IAvengerDebugWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetDebugMenuEntryData(UDebugMenuEntry* entryData);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnChangeValue(int32 values);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnApplyAction();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnCallDelegate();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSaveDebugValue();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnChangeNavigate();

	virtual FOnChangeHover* GetOnChangeHoverDelegate() = 0;
};
