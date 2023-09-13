// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Widget/AvengerDebugObjListWidget.h"

#include "AvengerDebugObjListChildElement.h"
#include "AvengerDebugObjListElement.h"
#include "Components/ListView.h"
#include "Serialization/ArchiveCountMem.h"

void UAvengerDebugObjListWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UAvengerDebugObjListWidget::NativeDestruct()
{
	Super::NativeDestruct();

	ClearListView();
}

void UAvengerDebugObjListWidget::ClearListView()
{
	if(IsValid(ListView))
	{
		ListView->ClearListItems();
		ListView->RequestRefresh();
	}
	
	ObjWidgetMap.Reset();
	SearchResultData.Reset();

	CurrentScrollIndex = 0;
}

void UAvengerDebugObjListWidget::SearchListView(const FString searchText)
{
	if(IsValid(ListView) == false)
		return;
	
	CurrentScrollIndex = 0;
	CurrentSearchText = searchText;
	SearchResultData.Reset();
	ListView->RequestRefresh();

	for(auto const& data : ObjWidgetMap)
	{
		if(data.Key->GetName().Contains(searchText))
		{
			SearchResultData.Add(data.Value);
			continue;
		}
		
		for(auto const& debugInfo : data.Value->MemUsageInfoArray)
		{
			if(debugInfo.ObjectName.Contains(searchText))
			{
				SearchResultData.Add(data.Value);
				break;
			}
		}
	}
}

void UAvengerDebugObjListWidget::RequestScroll(int value)
{
	if(IsValid(ListView) == false)
		return;
	
	if(SearchResultData.Num() > 0)
	{
		ListView->RequestNavigateToItem(SearchResultData[CurrentScrollIndex]);
		ListView->SetSelectedItem(SearchResultData[CurrentScrollIndex]);

		if(auto const childWidget = Cast<UAvengerDebugObjListElement>(ListView->GetEntryWidgetFromItem(SearchResultData[CurrentScrollIndex])))
		{
			if(childWidget->SearchListView(CurrentSearchText) && childWidget->RequestScroll(value))
				return;

			CurrentScrollIndex += value;
			CurrentScrollIndex = FMath::Clamp(CurrentScrollIndex, 0, SearchResultData.Num() - 1);
		}
	}
}

void UAvengerDebugObjListWidget::RefreshListView(const FOnSwitchingObjWidget& onSwitchingObjWidget, EDebugObjListSortType objListSortType)
{
	for(TObjectIterator<UObject> object; object; ++object)
	{
		if(object->IsValidLowLevelFast() == false)
			continue;
			
		// 破棄するものは無視
		if(object->GetClass()->HasAnyClassFlags(CLASS_Deprecated | CLASS_NewerVersionExists))
			continue;

		// UI、Widget関係(子供としてWidget追加しているので無限ループが起きてしまう)
		if(object->GetClass()->IsChildOf(UWidgetTree::StaticClass())
			|| object->GetClass()->IsChildOf(UVisual::StaticClass())
			|| object->GetClass()->IsChildOf(UWidget::StaticClass())
			|| object->GetClass()->ImplementsInterface(UAvengerDebugObjListInterface::StaticClass()))
				continue;
		
		// mem usage
		FArchiveCountMem archiveObjectMem(*object);
		FResourceSizeEx resSize(EResourceSizeMode::Exclusive);
		object->GetResourceSizeEx(resSize);
		FDebugUsageMemoryInfo memUsage(archiveObjectMem.GetNum(), archiveObjectMem.GetMax(), resSize.GetDedicatedSystemMemoryBytes(),
			resSize.GetDedicatedVideoMemoryBytes(), resSize.GetUnknownMemoryBytes(), resSize.GetTotalMemoryBytes(), *object);
			 
		if(ObjWidgetMap.Contains(object->GetClass()) == false)
		{
			auto const data = NewObject<UAvengerDebugObjListElementData>(this);
			data->ClassName = object->GetClass()->GetName();
			data->OnSwitchingObjWidget = onSwitchingObjWidget;
			data->SortType = objListSortType;
			ObjWidgetMap.Add(object->GetClass(), data);
		}

		ObjWidgetMap[object->GetClass()]->MemUsageInfoArray.Add(memUsage);
		ObjWidgetMap[object->GetClass()]->MemUsageTotal += (memUsage.ObjectMemoryMax / 1024.f);
		ObjWidgetMap[object->GetClass()]->ResMemUsageTotal += (resSize.GetTotalMemoryBytes() / 1024.f);
	}
	
	// ScrollBoxに追加
	SortListViewData(objListSortType);
	
	for(auto const &objWidget : ObjWidgetMap)
		ListView->AddItem(objWidget.Value);

	ListView->ScrollToTop();
	ListView->RequestRefresh();
}

void UAvengerDebugObjListWidget::SortListViewData(EDebugObjListSortType objListSortType)
{
	if(ObjWidgetMap.IsEmpty())
		return;

	ObjWidgetMap.ValueSort([objListSortType](const UAvengerDebugObjListElementData& a, const UAvengerDebugObjListElementData& b)
	{
		if(IsValid(&a) && IsValid(&b))
		{
			if(objListSortType == EDebugObjListSortType::MemUsage)
				return a.MemUsageTotal > b.MemUsageTotal;

			if(objListSortType == EDebugObjListSortType::ObjCount)
				return a.MemUsageInfoArray.Num() > b.MemUsageInfoArray.Num();

			return a.ClassName < b.ClassName;
		}

		UE_LOG(LogTemp, Log, TEXT("In AvengerDebugObjListWidget Can't Sort"));
		return false;
	});
}
