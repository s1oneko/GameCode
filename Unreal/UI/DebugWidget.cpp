// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


UDebugWidget::UDebugWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
}

void UDebugWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!bCanTick)
	{
		return;
	}
	//SetLocation();
	SetRelativeLocation();
}


void UDebugWidget::RPGDebugLog(FVector2D ScreenLocation, FString DebugContent, AActor* Target, FColor DebugColor,
                               float FontSize, float Duration)
{
	if (UWorld* World = Target->GetWorld())
	{
		if (UDebugWidget* DebugWid = CreateWidget<UDebugWidget>(World,DebugWidgetBP))
		{
			
			DebugWid->AttachedActor = Target;
			DebugWid->ScreenOffset = ScreenLocation;
			DebugWid->SetLocation();
			DebugWid->AddToViewport();
			DebugWid->bCanTick = true;
			
			if(UTextBlock* DebugText=DebugWid->DebugTextBlock)
			{
				const FString Content = Target->GetName() + ": " +DebugContent;
				DebugText->SetText(FText::FromString(Content));
				DebugText->SetColorAndOpacity(DebugColor);
				DebugText->SetFont(FSlateFontInfo(DebugText->GetFont().FontObject, FontSize));
			}
			
			
			World->GetTimerManager().SetTimer(DebugWid->HideTimerHandle, [DebugWid]
			{
				DebugWid->SetVisibility(ESlateVisibility::Hidden);
				DebugWid->RemoveFromParent();
			}, Duration, false);
		}
	}
}

void UDebugWidget::RPGDebugLog(FVector RelativeLocation, FString DebugContent, AActor* Target, FColor DebugColor,
	float FontSize, float Duration)
{
	if (UWorld* World = Target->GetWorld())
	{
		if (UDebugWidget* DebugWid = CreateWidget<UDebugWidget>(World,DebugWidgetBP))
		{
			
			DebugWid->AttachedActor = Target;
			//DebugWid->CalculateOffset(RelativeLocation);
			//DebugWid->SetLocation();
			DebugWid->RelativeOffset=RelativeLocation;
			DebugWid->SetRelativeLocation();
			DebugWid->AddToViewport();
			DebugWid->bCanTick = true;
			
			if(UTextBlock* DebugText=DebugWid->DebugTextBlock)
			{
				const FString Content = Target->GetName() + ": " +DebugContent;
				DebugText->SetText(FText::FromString(Content));
				DebugText->SetColorAndOpacity(DebugColor);
				DebugText->SetFont(FSlateFontInfo(DebugText->GetFont().FontObject, FontSize));
			}
			
			
			
			World->GetTimerManager().SetTimer(DebugWid->HideTimerHandle, [DebugWid]
			{
				DebugWid->SetVisibility(ESlateVisibility::Hidden);
				DebugWid->RemoveFromParent();
			}, Duration, false);
		}
	}
}

void UDebugWidget::RPGDebugLogScreen(FVector2D ScreenLocation, FString DebugContent, AActor* Target, FColor DebugColor,
                                     float FontSize, float Duration)
{
	RPGDebugLog(ScreenLocation, DebugContent, Target, DebugColor, FontSize, Duration);
}

void UDebugWidget::RPGDebugLogWorld(FVector RelativeLocation, FString DebugContent, AActor* Target, FColor DebugColor,
	float FontSize, float Duration)
{
	RPGDebugLog(RelativeLocation, DebugContent, Target, DebugColor, FontSize, Duration);
}

void UDebugWidget::SetLocation()
{
	FVector2D ScreenLocation;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation(), ScreenLocation))
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenLocation=(ScreenLocation+ScreenOffset)/Scale;
		SetPositionInViewport(ScreenLocation, false);
	}
}

void UDebugWidget::SetRelativeLocation()
{
	FVector2D ScreenLocation;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation()+RelativeOffset, ScreenLocation))
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenLocation/= Scale;
		SetPositionInViewport(ScreenLocation, false);
	}
}

void UDebugWidget::CalculateOffset(FVector Offset)
{
	FVector2D LocationWithOffset;
	UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation()+Offset, LocationWithOffset);
	FVector2D PivotLocation;
	UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation(), PivotLocation);
	ScreenOffset=LocationWithOffset-PivotLocation;
}


