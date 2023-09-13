// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AvengerDebugObjBaseWidget.h"
#include "AvengerDebugObjListInterface.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "AvengerDebugObjListTabWidget.generated.h"

/**
 * 
 */
UCLASS()
class AVENGER_CLIENT_API UAvengerDebugObjListTabWidget : public UUserWidget, public IAvengerDebugObjListInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnClosedButtonPress();

	UFUNCTION()
	void OnSwitchButtonPress();
	
	void SetTabLabel(FString tabLabel) const
	{
		TabLabel->SetText(FText::FromString(tabLabel));
	}

	void OnInitialized(const TWeakObjectPtr<UObject>& objectRef, const FOnClosingTabWidget& onClosingTabWidget, const FOnSwitchingObjWidget& onSwitchingObjWidget)
	{
		ObjectRef = objectRef;
		OnSwitchingObjWidget = onSwitchingObjWidget;
		OnClosingTabWidget = onClosingTabWidget;

		WidgetSwitchButton->OnClicked.AddDynamic(this, &UAvengerDebugObjListTabWidget::OnSwitchButtonPress);
		WidgetCloseButton->OnClicked.AddDynamic(this, &UAvengerDebugObjListTabWidget::OnClosedButtonPress);
	}

	TWeakObjectPtr<UObject> GetCurrentObjectRef() const { return ObjectRef; }

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UButton> WidgetSwitchButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> WidgetCloseButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> TabLabel;

	UPROPERTY(Transient)
	TWeakObjectPtr<UObject> ObjectRef;

	FOnClosingTabWidget OnClosingTabWidget;
	FOnSwitchingObjWidget OnSwitchingObjWidget;
};