// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Widget/AvengerDebugObjBaseWidget.h"

#include "AvengerDebugObjListTabWidget.h"
#include "AvengerDebugObjListWidget.h"
#include "AvengerDebugObjPropertyWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/WidgetSwitcher.h"


void UAvengerDebugObjBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(IsValid(ObjListWidgetClass) && ObjListWidget.IsValid() == false)
		ObjListWidget = CreateWidget<UAvengerDebugObjListWidget>(this, ObjListWidgetClass);

	if(IsValid(ObjPropertyWidgetClass) && ObjPropertyWidget.IsValid() == false)
		ObjPropertyWidget = CreateWidget<UAvengerDebugObjPropertyWidget>(this, ObjPropertyWidgetClass);

	OnSwitchingObjWidget.BindUFunction(this, "OnSwitchingTab");
	OnClosingPropertyWidget.BindUFunction(this, "OnClosedPropertyWidget");
	OnClosingTabWidget.BindUFunction(this, "OnClosedTabWidget");

	if(IsValid(WidgetSwitcher))
	{
		WidgetSwitcher->AddChild(ObjListWidget.Get());
		WidgetSwitcher->AddChild(ObjPropertyWidget.Get());
	}
}


void UAvengerDebugObjBaseWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	if(ObjListWidget.IsValid())
		ObjListWidget->RemoveFromParent();

	if(ObjPropertyWidget.IsValid())
		ObjPropertyWidget->RemoveFromParent();

	for(auto const& tabWidget : ObjListTabWidgets)
		tabWidget->RemoveFromParent();

	ObjListWidget = nullptr;
	ObjPropertyWidget = nullptr;
	ObjListTabWidgets.Reset();
}

void UAvengerDebugObjBaseWidget::OnPressSearchButton(const FString searchText)
{
	if(IsValid(WidgetSwitcher->GetActiveWidget()) == false)
		return;

	if(WidgetSwitcher->GetActiveWidget() == ObjListWidget)
		ObjListWidget->SearchListView(searchText);

	if(WidgetSwitcher->GetActiveWidget() == ObjPropertyWidget)
		ObjPropertyWidget->SearchListView(searchText);
}

void UAvengerDebugObjBaseWidget::OnPressSearchIndexButton(int value)
{
	if(IsValid(WidgetSwitcher->GetActiveWidget()) == false)
		return;

	if(WidgetSwitcher->GetActiveWidget() == ObjListWidget)
		ObjListWidget->RequestScroll(value);

	if(WidgetSwitcher->GetActiveWidget() == ObjPropertyWidget)
		ObjPropertyWidget->RequestScroll(value);
}



void UAvengerDebugObjBaseWidget::OnPressRefreshButton(EDebugObjListSortType objListSortType)
{
	if(ObjListWidget.IsValid())
	{
		ObjListWidget->ClearListView();
		ObjListWidget->RefreshListView(OnSwitchingObjWidget, objListSortType);
		WidgetSwitcher->SetActiveWidget(ObjListWidget.Get());
	}
}


void UAvengerDebugObjBaseWidget::OnSwitchingTab(UObject* objectRef)
{
	if(IsValid(objectRef) && ObjPropertyWidget.IsValid())
	{
		ObjPropertyWidget->ClearListView();
		ObjPropertyWidget->RequestProperty(objectRef);
		WidgetSwitcher->SetActiveWidget(ObjPropertyWidget.Get());

		for(auto const& tabWidget : ObjListTabWidgets)
		{
			if(tabWidget->GetCurrentObjectRef() == objectRef)
				return;
		}

        auto const tabWidget = CreateWidget<UAvengerDebugObjListTabWidget>(this, ObjTabWidgetClass);
        tabWidget->SetTabLabel(objectRef->GetName());
        tabWidget->OnInitialized(objectRef, OnClosingTabWidget, OnSwitchingObjWidget);
        TabHorizontalBox->AddChildToHorizontalBox(tabWidget);
        ObjListTabWidgets.Add(tabWidget);
	}
}

void UAvengerDebugObjBaseWidget::OnClosedTabWidget(UAvengerDebugObjListTabWidget* widgetRef)
{
	if(IsValid(widgetRef) && ObjListTabWidgets.Contains(widgetRef))
		ObjListTabWidgets.Remove(widgetRef);

	widgetRef->RemoveFromParent();
	
	if(widgetRef->GetCurrentObjectRef() == ObjPropertyWidget->GetCurrentObjectRef())
	{
		if(ObjPropertyWidget.IsValid())
			ObjPropertyWidget->ClearListView();
		
		WidgetSwitcher->SetActiveWidget(ObjListWidget.Get());
	}
}

void UAvengerDebugObjBaseWidget::OnClosedPropertyWidget()
{
	if(ObjPropertyWidget.IsValid())
		ObjPropertyWidget->ClearListView();
		
	if(ObjListWidget.IsValid())
		WidgetSwitcher->SetActiveWidget(ObjListWidget.Get());
}