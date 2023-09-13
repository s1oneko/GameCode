// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "AvengerDebugPerformanceWidget.generated.h"

USTRUCT()
struct FDebugPerformance
{
	GENERATED_BODY()

	FDebugPerformance() { }
	FDebugPerformance(FString threadName, TObjectPtr<class UImage> barWidget, TObjectPtr<class UTextBlock> textWidget)
	: ThreadName(threadName), BarWidget(barWidget), TextWidget(textWidget) { }

	UPROPERTY(Transient)
	FString ThreadName;
	
	UPROPERTY(Transient)
	TObjectPtr<UImage> BarWidget;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> TextWidget;

	UPROPERTY(Transient)
	float FrameMs = 0.f;
};

/**
 * 
 */
UCLASS()
class AVENGER_CLIENT_API UAvengerDebugPerformanceWidget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdatePerformanceWidget();
	void ChangeWidgetParam();

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> FPSBar;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> FPSTextBlock;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> DrawBar;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> DrawTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> GameBar;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> GameTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> GPUBar;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> GPUTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> SwapBar;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> SwapTextBlock;

	UPROPERTY(Transient)
	TArray<FDebugPerformance> DebugPerformances;
};