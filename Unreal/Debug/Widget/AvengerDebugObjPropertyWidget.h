// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AvengerDebugObjListInterface.h"
#include "Blueprint/UserWidget.h"
#include "AvengerDebugObjPropertyWidget.generated.h"

/**
 * 
 */
UCLASS()
class AVENGER_CLIENT_API UAvengerDebugObjPropertyWidget : public UUserWidget, public IAvengerDebugObjListInterface
{
	GENERATED_BODY()

public:
	virtual void NativeDestruct() override;
	
	void RequestProperty(TWeakObjectPtr<UObject> objectRef);
	void ClearListView();

	void SearchListView(const FString searchText);
	void RequestScroll(int value);

	TWeakObjectPtr<UObject> GetCurrentObjectRef() const { return ObjectRef; }

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UListView> PropertyListView;

	UPROPERTY(Transient)
	TArray<FString> PropertyValueArray;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<class UAvengerDebugObjPropertyElementData>> ElementDataArray;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<UAvengerDebugObjPropertyElementData>> SearchResultDataArray;

	UPROPERTY(Transient)
	TWeakObjectPtr<UObject> ObjectRef;

	UPROPERTY(Transient)
	int CurrentScrollIndex = 0;
};