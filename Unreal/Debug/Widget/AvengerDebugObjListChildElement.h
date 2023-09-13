// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AvengerDebugObjBaseWidget.h"
#include "AvengerDebugObjListElement.h"
#include "AvengerDebugObjListInterface.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "AvengerDebugObjListChildElement.generated.h"

/**
 * 
 */
UCLASS()
class AVENGER_CLIENT_API UAvengerDebugObjListChildElement : public UUserWidget, public IAvengerDebugObjListInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable)
	void OnPressElemButton();

public:
	int32 GetObjectMemSize() const
	{
		if(ObjMemoryUsageInfo.ObjectRef.IsValid())
			return ObjMemoryUsageInfo.ObjectMemoryMax / 1024.f;

		return 0;
	}
	FString GetObjectName() const
	{
		if(ObjMemoryUsageInfo.ObjectRef.IsValid())
			return ObjMemoryUsageInfo.ObjectRef->GetName();

		return "";
	}

	void SetWidgetParam(const FDebugUsageMemoryInfo& objMemoryUsageInfo, const FOnSwitchingObjWidget& onSwitchingObjWidget);
	void SelectWidget(bool isSelected) const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UAvengerDebugObjListHoverWidget> HoverWidgetClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> ElemButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> ObjectNumText;

	UPROPERTY(Transient)
	TWeakObjectPtr<UAvengerDebugObjListHoverWidget> HoverWidget;
	
	UPROPERTY(Transient)
	FDebugUsageMemoryInfo ObjMemoryUsageInfo;

	FOnSwitchingObjWidget OnSwitchingObjWidget;
};