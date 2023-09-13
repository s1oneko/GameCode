// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "AvengerDebugDefaultWidget.generated.h"


/**
 * 
 */
// Default カテゴリー用のUIWidget
UCLASS()
class AVENGER_CLIENT_API UAvengerDebugDefaultWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	
public:
	void SetFPSVisibility(uint8 isVisibility) const;
	void SetInputBindVisibility(uint8 isVisibility) const;
	void SetPerformanceVisibility(uint8 isVisibility) const;

protected:
	UPROPERTY(Transient, meta = (BindWidgetOptional))
	TObjectPtr<class UCanvasPanel> FPSPanel;

	UPROPERTY(Transient, meta = (BindWidgetOptional))
	TObjectPtr<UCanvasPanel> InputPanel;

	UPROPERTY(Transient, meta = (BindWidgetOptional))
	TObjectPtr<UCanvasPanel> PerformancePanel;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag FPSDebugTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputDebugTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PerformanceTag;
};