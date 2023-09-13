// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/DebugLayoutWidget.h"

#include "AvengerDebugSettings.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Debug/Widget/AvengerDebugLogWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Overlay.h"

UAvengerDebugLogWidget* UDebugLayoutWidget::CreateDebugLogWidget(UWorld* world)
{
	if(IsValid(world))
	{
		if(!GetDefault<UAvengerDebugSettings>()->DebugLogClass.IsNull())
		{
			auto const debugWidgetClass = GetDefault<UAvengerDebugSettings>()->DebugLogClass;
			if(auto const debugWidget = debugWidgetClass.LoadSynchronous())
			{
				if(UAvengerDebugLogWidget* DebugWid = CreateWidget<UAvengerDebugLogWidget>(world, debugWidget))
				{
					DebugWid->AddToViewport();
					return DebugWid;
				}
			}
		}
	}
	return nullptr;
}


UOverlay* UDebugLayoutWidget::GetLayout()
{
	return Layout;
}


//array
UAvengerDebugLogWidget* UDebugLayoutWidget::GetText()
{
	if (TextPool.Num() > 0)
	{
		UAvengerDebugLogWidget* Obj = TextPool.Pop();
		return Obj;
	}
	UAvengerDebugLogWidget* newText = CreateDebugLogWidget(GetWorld());
	return newText;
}

void UDebugLayoutWidget::ReturnText(UAvengerDebugLogWidget* text)
{
	if (text)
	{
		TextPool.Add(text);
	}
}
