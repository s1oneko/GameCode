// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Widget/AvengerDebugObjListChildElement.h"

#include "AvengerDebugObjListHoverWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UAvengerDebugObjListChildElement::NativeConstruct()
{
	Super::NativeConstruct();

	if(IsValid(ElemButton))
		ElemButton->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));
}


void UAvengerDebugObjListChildElement::NativeDestruct()
{
	Super::NativeDestruct();

	if(HoverWidget.IsValid())
		HoverWidget = nullptr;
}

void UAvengerDebugObjListChildElement::OnPressElemButton()
{
	if(OnSwitchingObjWidget.IsBound())
		OnSwitchingObjWidget.Execute(ObjMemoryUsageInfo.ObjectRef.Get());
}

void UAvengerDebugObjListChildElement::SelectWidget(bool isSelected) const
{
	if(IsValid(ElemButton))
		ElemButton->SetBackgroundColor(isSelected ? FLinearColor(1.f, 1.f, 0.f, 1.f) : FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));
}


void UAvengerDebugObjListChildElement::SetWidgetParam(const FDebugUsageMemoryInfo& objMemoryUsageInfo, const FOnSwitchingObjWidget& onSwitchingObjWidget)
{
	if(objMemoryUsageInfo.ObjectRef.IsValid() == false)
		return;

	if(HoverWidgetClass && HoverWidget.IsValid() == false)
		HoverWidget = CreateWidget<UAvengerDebugObjListHoverWidget>(this, HoverWidgetClass);

	ObjMemoryUsageInfo = objMemoryUsageInfo;
	OnSwitchingObjWidget = onSwitchingObjWidget;
	HoverWidget->SetHoverInfo(ObjMemoryUsageInfo);

	if(IsValid(ElemButton))
		ElemButton->SetToolTip(HoverWidget.Get());
	
	ObjectNumText->SetText(FText::FromString(ObjMemoryUsageInfo.ObjectRef->GetName()));
}