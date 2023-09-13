// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AvengerDebugObjListInterface.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "AvengerDebugObjPropertyElement.generated.h"

UCLASS()
class UAvengerDebugObjPropertyElementData : public UObject, public IAvengerDebugObjListInterface
{
	GENERATED_BODY()

public:
	FString PropertyValue;
};

/**
 * 
 */
UCLASS()
class AVENGER_CLIENT_API UAvengerDebugObjPropertyElement : public UUserWidget, public IUserObjectListEntry, public IAvengerDebugObjListInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> StrValueText;
};