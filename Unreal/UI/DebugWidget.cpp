// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/UI/Widget/DebugWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


UDebugWidget::UDebugWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	
}



void UDebugWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(!IsValid(DebugLogTicker))
		DebugLogTicker = NewObject<UDebugLogTicker>(this);

	DebugLogTicker->GetDebugLogOnTick().AddDynamic(this,&UDebugWidget::SetLocation);
	DebugLogTicker->GetDebugOnEnd().AddDynamic(this, &UDebugWidget::OnEnd);
}

void UDebugWidget::NativeDestruct()
{
	DebugLogTicker->GetDebugLogOnTick().RemoveDynamic(this, &UDebugWidget::SetLocation);
	DebugLogTicker->GetDebugOnEnd().RemoveDynamic(this, &UDebugWidget::OnEnd);
	
	Super::NativeDestruct();
}

void UDebugWidget::OnEnd()
{
	RemoveFromParent();
}

void UDebugWidget::ConfigSetting(FVector2D screenLocation, FString debugContent, AActor* target,
	FColor debugColor, float fontSize, float duration)
{
	ScreenLocation = screenLocation;
	bIs2D = true;
	
	SetLocation();
	DebugLogTicker->SetElapsedTimeMax(duration);
	
	if(DebugTextBlock)
	{
		const FString Content = target->GetName() + ": " +debugContent;
		DebugTextBlock->SetText(FText::FromString(Content));
		DebugTextBlock->SetColorAndOpacity(debugColor);
		DebugTextBlock->SetFont(FSlateFontInfo(DebugTextBlock->GetFont().FontObject, fontSize));
	}
}


void UDebugWidget::ConfigSetting(FVector worldLocation, FString debugContent, AActor* target,
	FColor debugColor, float fontSize, float duration)
{
	WorldLocation = worldLocation;
	bIs2D = false;
	
	SetLocation();
	DebugLogTicker->SetElapsedTimeMax(duration);

	if(DebugTextBlock)
	{
		const FString Content = target->GetName() + ": " +debugContent;
		DebugTextBlock->SetText(FText::FromString(Content));
		DebugTextBlock->SetColorAndOpacity(debugColor);
		DebugTextBlock->SetFont(FSlateFontInfo(DebugTextBlock->GetFont().FontObject, fontSize));
	}
}

void UDebugWidget::SetLocation()
{
	if(bIs2D)
	{
		SetPositionInViewport(ScreenLocation, false);
		return;
	}
	FVector2D screenLocation;
	float scale = UWidgetLayoutLibrary::GetViewportScale(this);
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), WorldLocation, screenLocation))
	{
		screenLocation /= scale;
		SetPositionInViewport(screenLocation, false);
	}
}



