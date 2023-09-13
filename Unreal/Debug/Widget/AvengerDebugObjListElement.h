// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AvengerDebugObjBaseWidget.h"
#include "AvengerDebugObjListInterface.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "AvengerDebugObjListElement.generated.h"


USTRUCT()
struct FDebugUsageMemoryInfo
{
	GENERATED_BODY()

	FDebugUsageMemoryInfo() {}
	FDebugUsageMemoryInfo(int32 objMemNum, int32 objMemMax, int32 sysResMem, int32 videoResMem, int32 unknownResMem, int32 totalMem, TWeakObjectPtr<UObject> objectRef) :
	ObjectMemoryNum(objMemNum), ObjectMemoryMax(objMemMax), SysResMemory(sysResMem), VideoResMemory(videoResMem),
	UnknownResMemory(unknownResMem), TotalResMemory(totalMem), ObjectRef(objectRef)
	{
		if(ObjectRef.IsValid())
			ObjectName = ObjectRef->GetName();
	}
	
	UPROPERTY(Transient)
	int32 ObjectMemoryNum;

	UPROPERTY(Transient)
	int32 ObjectMemoryMax;

	UPROPERTY(Transient)
	int32 SysResMemory;

	UPROPERTY(Transient)
	int32 VideoResMemory;

	UPROPERTY(Transient)
	int32 UnknownResMemory;

	UPROPERTY(Transient)
	int32 TotalResMemory;

	UPROPERTY(Transient)
	FString ObjectName;
	
	TWeakObjectPtr<UObject> ObjectRef;
};

UCLASS()
class UAvengerDebugObjListElementData : public UObject, public IAvengerDebugObjListInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	FString ClassName;

	UPROPERTY(Transient)
	int32 MemUsageTotal;

	UPROPERTY(Transient)
	int32 ResMemUsageTotal;

	UPROPERTY(Transient)
	TArray<FDebugUsageMemoryInfo> MemUsageInfoArray;

	UPROPERTY(Transient)
	EDebugObjListSortType SortType;
	
	FOnSwitchingObjWidget OnSwitchingObjWidget;
};

/**
 * 
 */
UCLASS()
class AVENGER_CLIENT_API UAvengerDebugObjListElement : public UUserWidget, public IUserObjectListEntry, public IAvengerDebugObjListInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

public:
	int32 GetAllChildMemUsageSize() const { return AllChildMemUsageSize; }
	bool SearchListView(FString searchText);
	bool RequestScroll(int value);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UAvengerDebugObjListChildElement> ChildWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UScrollBox> ChildScrollBox;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> ItemTextBlock;
	
	UPROPERTY(Transient)
	int32 AllChildMemUsageSize = 0;

	UPROPERTY(Transient)
	int32 AllChildResUsageSize = 0;
	
	UPROPERTY(Transient)
	TArray<TObjectPtr<class UAvengerDebugObjListChildElement>> SearchResultWidget;

	UPROPERTY(Transient)
	TArray<TObjectPtr<class UAvengerDebugObjListChildElement>> ChildWidgets;

	UPROPERTY(Transient)
	int CurrentScrollIndex = 0;

	UPROPERTY(Transient)
	TWeakObjectPtr<UAvengerDebugObjListChildElement> CurrentWidget;

	UPROPERTY(Transient)
	bool bIsSearchFinish = false;
};