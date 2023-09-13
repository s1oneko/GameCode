// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Widget/AvengerDebugDefaultWidget.h"

#include "Components/CanvasPanel.h"
#include "Debug/DebugMenuSubsystem.h"

void UAvengerDebugDefaultWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if(GetWorld())
	{
		if(auto const debugSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UDebugMenuSubsystem>())
		{
			SetFPSVisibility(debugSubsystem->GetValue(FPSDebugTag));
			SetInputBindVisibility(debugSubsystem->GetValue(InputDebugTag));
			SetPerformanceVisibility(debugSubsystem->GetValue(PerformanceTag));
		}
	}
}


void UAvengerDebugDefaultWidget::SetInputBindVisibility(uint8 isVisibility) const
{
	if(IsValid(InputPanel))
		InputPanel->SetVisibility(isVisibility ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UAvengerDebugDefaultWidget::SetFPSVisibility(uint8 isVisibility) const
{
	if(IsValid(FPSPanel))
		FPSPanel->SetVisibility(isVisibility ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UAvengerDebugDefaultWidget::SetPerformanceVisibility(uint8 isVisibility) const
{
	if(IsValid(PerformancePanel))
		PerformancePanel->SetVisibility(isVisibility ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}
