// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugLayoutWidget.generated.h"

/**
 * 
 */

UCLASS()
class AVENGER_CLIENT_API UDebugLayoutWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<class UAvengerDebugLogWidget*> TextPool;

public:

	UFUNCTION()
	class UOverlay* GetLayout();

	//array
	UFUNCTION()
	UAvengerDebugLogWidget* GetText();

	void ReturnText(UAvengerDebugLogWidget* text);
	
	UFUNCTION()
	static UAvengerDebugLogWidget* CreateDebugLogWidget(UWorld* world);

protected:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* Layout;
	
	
};
