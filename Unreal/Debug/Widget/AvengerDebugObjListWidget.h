// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AvengerDebugObjBaseWidget.h"
#include "AvengerDebugObjListInterface.h"
#include "Blueprint/UserWidget.h"
#include "AvengerDebugObjListWidget.generated.h"

/**
 * 
 */
// EUWでobjlistと同じようなListView用Widget
UCLASS()
class AVENGER_CLIENT_API UAvengerDebugObjListWidget : public UUserWidget, public IAvengerDebugObjListInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	void RefreshListView(const FOnSwitchingObjWidget& onSwitchingObjWidget, EDebugObjListSortType objListSortType = EDebugObjListSortType::MemUsage);
	void SortListViewData(EDebugObjListSortType objListSortType = EDebugObjListSortType::MemUsage);
	void ClearListView();
	
	void SearchListView(const FString searchText);
	void RequestScroll(int value);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UListView> ListView;

	UPROPERTY(Transient)
	TMap<UClass*, TObjectPtr<class UAvengerDebugObjListElementData>> ObjWidgetMap;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UAvengerDebugObjListElementData>> SearchResultData;

	UPROPERTY(Transient)
	int CurrentScrollIndex = 0;

	UPROPERTY(Transient)
	FString CurrentSearchText;
};