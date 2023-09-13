// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Widget/AvengerDebugWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Debug/AvengerDebugHelper.h"
#include "Input/CommonUIInputTypes.h"

void UAvengerDebugWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ApplyHandle = RegisterUIActionBinding(FBindUIActionArgs(ApplyInputActionData, false, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleApplyAction)));
}

UWidget* UAvengerDebugWidget::NativeGetDesiredFocusTarget() const
{
	if(CurrentFocusMenu.IsValid())
		return CurrentFocusMenu.Get();
	
	if(IsValid(CurrentMenu))
		return CurrentMenu->GetChildAt(0);

	return BP_GetDesiredFocusTarget();
}

void UAvengerDebugWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	RequestCreateDynamicTab();
}

void UAvengerDebugWidget::HandleApplyAction()
{
	if(CurrentFocusMenu.IsValid())
	{
		if(CurrentFocusMenu->GetClass()->ImplementsInterface(UAvengerDebugWidgetInterface::StaticClass()))
			IAvengerDebugWidgetInterface::Execute_OnApplyAction(CurrentFocusMenu.Get());
	}
}


void UAvengerDebugWidget::ClearDebugMenu()
{
	for(auto const& tuple : MenuMap)
	{
		for(auto const& item : tuple.Value->GetAllChildren())
			item->RemoveFromParent();

		tuple.Value->RemoveFromParent();
	}

	TopDebugMenuTabs->RemoveAllTabs();
	MenuMap.Reset();
}


void UAvengerDebugWidget::AddDebugMenu(const TArray<UDebugMenuEntry*>& debugMenuEntry)
{
	if(debugMenuEntry.IsEmpty() || !IsValid(MenuBox))
		return;

	// TODO :
	ClearDebugMenu();

	// menu list
	for(auto const& menuEntry : debugMenuEntry)
	{
		auto const menuItem = CreateWidget<UCommonButtonBase>(GetOwningPlayer(),
			menuEntry->MenuType == EDebugMenuType::Enum ? EnumItemMenuClass.LoadSynchronous() : ItemMenuClass.LoadSynchronous());

		if(menuItem->GetClass()->ImplementsInterface(UAvengerDebugWidgetInterface::StaticClass()))
		{
			IAvengerDebugWidgetInterface::Execute_SetDebugMenuEntryData(menuItem, menuEntry);

			IAvengerDebugWidgetInterface* widgetInterface = Cast<IAvengerDebugWidgetInterface>(menuItem);

			if(FOnChangeHover* hoverDelegate = widgetInterface->GetOnChangeHoverDelegate())
				(*hoverDelegate).AddDynamic(this, &UAvengerDebugWidget::OnChangeHoverMenu);
		}
		
		if(MenuMap.Contains(menuEntry->Category))
		{
			MenuMap[menuEntry->Category]->AddChildToVerticalBox(menuItem);
			continue;
		}

		auto const childMenuBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
		MenuBox->AddChildToHorizontalBox(childMenuBox);
		childMenuBox->AddChildToVerticalBox(menuItem);
		childMenuBox->SetVisibility(ESlateVisibility::Collapsed);
		MenuMap.Add(menuEntry->Category, childMenuBox);
	}

	// tab
	RequestCreateDynamicTab();
}

void UAvengerDebugWidget::RequestCreateDynamicTab()
{
	if(!IsValid(TopDebugMenuTabs))
		return;

	if(TopDebugMenuTabs->GetVisibleTabCount() != 0)
		return;

	for(auto const& tuple : MenuMap)
	{
		if(IsValid(TopDebugMenuTabs) && !TopMenuTabClass.IsNull())
		{
			FAvengerTabDescriptor tabDescriptor;
			tabDescriptor.TabId = FName(tuple.Key);
			tabDescriptor.TabText = FText::FromString(tuple.Key);
			tabDescriptor.TabButtonType = TopMenuTabClass.LoadSynchronous();
			TopDebugMenuTabs->RegisterDynamicTab(tabDescriptor);
		}
	}
}


void UAvengerDebugWidget::NavigateMenu(FString tabID)
{
	if(tabID.IsEmpty() || !MenuMap.Contains(tabID))
		return;

	if(IsValid(CurrentMenu))
		CurrentMenu->SetVisibility(ESlateVisibility::Collapsed);

	MenuMap[tabID]->SetVisibility(ESlateVisibility::Visible);

	for(auto const& child : MenuMap[tabID]->GetAllChildren())
	{
		if(child->GetClass()->ImplementsInterface(UAvengerDebugWidgetInterface::StaticClass()))
			IAvengerDebugWidgetInterface::Execute_OnChangeNavigate(child);
	}
	
	CurrentMenu = MenuMap[tabID];
}

void UAvengerDebugWidget::OnChangeHoverMenu(UCommonButtonBase* target)
{
	if(!IsValid(target))
		return;

	CurrentFocusMenu = target;
}