// Fill out your copyright notice in the Description page of Project Settings.


#include "AvengerLayoutWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Debug/Widget/AvengerDebugLogWidget.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

UAvengerLayoutWidget::UAvengerLayoutWidget(const FObjectInitializer & ObjectInitializer):Super(ObjectInitializer)
{
	
}

void UAvengerLayoutWidget::RegisterOverlay(UActorComponent* component, UAvengerDebugLogWidget* overlay)
{
	if (component)
	{
		LayoutMap.Add(component, overlay);
		VerticalBox->AddChild(overlay);
	}
}

UAvengerDebugLogWidget* UAvengerLayoutWidget::GetOverlay(UActorComponent* component)
{
	if(component)
	{
		if (LayoutMap.Contains(component))
		{
			return LayoutMap.FindRef(component);
		}
	}
	return nullptr;
}

void UAvengerLayoutWidget::RemoveFromMap(UActorComponent* component)
{
	if(component && LayoutMap.Contains(component))
	{
		if(UAvengerDebugLogWidget* toRemove = LayoutMap[component])
		{
			toRemove->RemoveFromParent(); 
			toRemove->ConditionalBeginDestroy();
		}
		LayoutMap.Remove(component);
	}
}
void UAvengerLayoutWidget::SetLocation(FVector Location)
{
	
	FVector2D convertLocation;
	const float scale = UWidgetLayoutLibrary::GetViewportScale(this);

	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayerCameraManager()->GetOwningPlayerController(), Location, convertLocation))
	{
		convertLocation /= scale;
		SetPositionInViewport(convertLocation, false);
	}
}
