// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "AvengerDebugFPSWidget.generated.h"

/**
 * 
 */
UCLASS()
class AVENGER_CLIENT_API UAvengerDebugFPSWidget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> FPSText;
};
