// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Widget/AvengerDebugObjListElement.h"

#include "Components/ScrollBox.h"
#include "Debug/Widget/AvengerDebugObjListChildElement.h"

#include "Components/TextBlock.h"


void UAvengerDebugObjListElement::NativeDestruct()
{
	Super::NativeDestruct();

	if(IsValid(ChildScrollBox))
		ChildScrollBox->ClearChildren();

	CurrentScrollIndex = 0;
	
	SearchResultWidget.Reset();
	ChildWidgets.Reset();
}

bool UAvengerDebugObjListElement::SearchListView(FString searchText)
{
	if(SearchResultWidget.IsEmpty())
	{
		for(auto const& item : ChildScrollBox->GetAllChildren())
		{
			auto const widget = Cast<UAvengerDebugObjListChildElement>(item);

			if(widget->GetObjectName().Contains(searchText))
				SearchResultWidget.Add(widget);
		}
	}
	
	return SearchResultWidget.Num() > 0;
}

bool UAvengerDebugObjListElement::RequestScroll(int value)
{
	if(CurrentWidget.IsValid())
		CurrentWidget->SelectWidget(false);
	
	ChildScrollBox->ScrollWidgetIntoView(SearchResultWidget[CurrentScrollIndex]);
	SearchResultWidget[CurrentScrollIndex]->SelectWidget(true);
	
	if(CurrentScrollIndex + value < 0 || CurrentScrollIndex + value >= SearchResultWidget.Num())
		return false;

	CurrentWidget = SearchResultWidget[CurrentScrollIndex];
	CurrentScrollIndex += value;
	return true;
}

void UAvengerDebugObjListElement::NativeOnItemSelectionChanged(bool bIsSelected)
{
	if(bIsSelected == false)
	{
		for(auto const& widget : SearchResultWidget)
			widget->SelectWidget(bIsSelected);
	}
}


void UAvengerDebugObjListElement::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	auto const obj = Cast<UAvengerDebugObjListElementData>(ListItemObject);
	
	AllChildMemUsageSize = obj->MemUsageTotal;
	AllChildResUsageSize = obj->ResMemUsageTotal;
	CurrentScrollIndex = 0;
	CurrentWidget = nullptr;
	SearchResultWidget.Reset();
   	ChildWidgets.Reset();
	ChildScrollBox->ClearChildren();

	for(auto const& data : obj->MemUsageInfoArray)
	{
		if(IsValid(ChildWidgetClass))
		{
			auto const childWidget = CreateWidget<UAvengerDebugObjListChildElement>(this, ChildWidgetClass);
			childWidget->SetWidgetParam(data, obj->OnSwitchingObjWidget);
			ChildWidgets.Add(childWidget);
		}
	}
		
	ChildWidgets.Sort([obj](const UAvengerDebugObjListChildElement& a, const UAvengerDebugObjListChildElement& b)
	{
		if(IsValid(&a) && IsValid(&b))
		{
			if(obj->SortType == EDebugObjListSortType::Alphanumerical)
				return a.GetObjectName() < b.GetObjectName();
			
			return a.GetObjectMemSize() / 1024.f > b.GetObjectMemSize() / 1024.f;
		}
		
		UE_LOG(LogTemp, Log, TEXT("In AvengerDebugObjListElement Can't Sort"));
		return false;
	});
	
	for(auto const& widget : ChildWidgets)
		ChildScrollBox->AddChild(widget);
	
	ItemTextBlock->SetText(FText::FromString(obj->ClassName + "\n(Count : " + FString::FromInt(ChildWidgets.Num()) + ")"
		+ "\n(Memory Usage : " + FString::FromInt(AllChildMemUsageSize)+ " KB)\n"
		+ "(Res Memory Usage : " + FString::FromInt(AllChildResUsageSize) + " KB)"));
}