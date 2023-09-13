// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AvengerDebugObjListElement.h"
#include "Blueprint/UserWidget.h"
#include "AvengerDebugObjListHoverWidget.generated.h"

/**
 * 
 */
UCLASS()
class AVENGER_CLIENT_API UAvengerDebugObjListHoverWidget : public UUserWidget, public IAvengerDebugObjListInterface
{
	GENERATED_BODY()

public:
	void SetHoverInfo(const FDebugUsageMemoryInfo debugUsageMemory) const;

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> InfoText;
	
};
