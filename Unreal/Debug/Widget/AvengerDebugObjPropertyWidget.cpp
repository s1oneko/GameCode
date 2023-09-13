// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Widget/AvengerDebugObjPropertyWidget.h"

#include "AvengerDebugObjPropertyElement.h"
#include "Components/ListView.h"


void UAvengerDebugObjPropertyWidget::NativeDestruct()
{
	ClearListView();
}

void UAvengerDebugObjPropertyWidget::ClearListView()
{
	if(IsValid(PropertyListView))
		PropertyListView->ClearListItems();
	
	ElementDataArray.Reset();
	PropertyValueArray.Reset();
	SearchResultDataArray.Reset();

	CurrentScrollIndex = 0;
}

void UAvengerDebugObjPropertyWidget::SearchListView(const FString searchText)
{
	if(ElementDataArray.IsEmpty())
		return;

	SearchResultDataArray.Reset();
	CurrentScrollIndex = 0;
	
	for(auto const& data : ElementDataArray)
	{
		if(data->PropertyValue.Contains(searchText))
			SearchResultDataArray.Add(data);
	}
}

void UAvengerDebugObjPropertyWidget::RequestScroll(int value)
{
	if(SearchResultDataArray.IsEmpty())
		return;
	
	PropertyListView->RequestNavigateToItem(SearchResultDataArray[CurrentScrollIndex].Get());
	CurrentScrollIndex += value;
	CurrentScrollIndex = FMath::Clamp(CurrentScrollIndex, 0, SearchResultDataArray.Num() - 1);
}


void UAvengerDebugObjPropertyWidget::RequestProperty(TWeakObjectPtr<UObject> objectRef)
{
	if(objectRef.IsValid() == false)
		return;
	
	for(TFieldIterator<FProperty> lt(objectRef->GetClass()); lt; ++lt)
	{
		FString tmpStr;
		lt->ExportText_InContainer(0, tmpStr, objectRef.Get(), objectRef.Get(), objectRef.Get(), PPF_IncludeTransient);
		FString const resultStr = lt->GetName() + ":\n" + tmpStr + "\n";
		PropertyValueArray.Add(resultStr);
	}
	
	for(auto const& str : PropertyValueArray)
	{
		auto const data = NewObject<UAvengerDebugObjPropertyElementData>(this);
		data->PropertyValue = str;
		PropertyListView->AddItem(data);
		ElementDataArray.Add(data);
	}

	ObjectRef = objectRef;
	PropertyListView->ScrollToTop();
}