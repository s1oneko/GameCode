// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Widget/AvengerDebugLogWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TextBlock.h"
#include "Debug/AvengerDebugSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Ui/Common/AvengerUIExtensions.h"

UAvengerDebugLogWidget::UAvengerDebugLogWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
}

// void UAvengerDebugLogWidget::NativeConstruct()
// {
// 	Super::NativeConstruct();
// }


void UAvengerDebugLogWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(!IsValid(DebugLogTicker))
		DebugLogTicker = NewObject<UDebugLogTicker>(this);

	DebugLogTicker->GetDebugLogOnTick().AddDynamic(this, &UAvengerDebugLogWidget::SetLocation);
	DebugLogTicker->GetDebugOnEnd().AddDynamic(this, &UAvengerDebugLogWidget::OnEnd);
}

void UAvengerDebugLogWidget::NativeDestruct()
{
	DebugLogTicker->GetDebugLogOnTick().RemoveDynamic(this, &UAvengerDebugLogWidget::SetLocation);
	DebugLogTicker->GetDebugOnEnd().RemoveDynamic(this, &UAvengerDebugLogWidget::OnEnd);
	
	Super::NativeDestruct();
}

void UAvengerDebugLogWidget::OnEnd()
{
	RemoveFromParent();
}


// void UAvengerDebugLogWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
// {
// 	Super::NativeTick(MyGeometry, InDeltaTime);
// 	if (!bCanTick||GetWorld()&&GetWorld()->IsPaused())
// 	{
// 		return;
// 	}
// 	if (bIsTimerActive)
// 	{
// 		ElapsedTime += InDeltaTime;
// 		if (ElapsedTime >= TimerDuration)
// 		{
// 			OnTimerTriggered.Broadcast();
// 			ElapsedTime = 0.0f;
// 		}
// 	}
// 	
// 	UE_LOG(LogTemp,Log,TEXT("%s, %f"),*GetName(), ElapsedTime);
// }

void UAvengerDebugLogWidget::ConfigSetting(FVector2D ScreenLocation, FString DebugContent, AActor* Target,
	FColor DebugColor, float FontSize, float Duration, bool IsRelativePosition)
{
	// TODO : 同じ処理する部分は関数とかで求められないか検討してください。
	AttachedActor = Target;
	ScreenOffset = ScreenLocation;
	RelativeOffset = FVector::Zero();
	bIsRelativePosition =IsRelativePosition;
	bIs2D = true;
	SetLocation();
	// TimerDuration = Duration;
	DebugLogTicker->SetElapsedTimeMax(Duration);
					
	if(DebugTextBlock)
	{
		const FString Content = Target->GetName() + ": " +DebugContent;
		DebugTextBlock->SetText(FText::FromString(Content));
		DebugTextBlock->SetColorAndOpacity(DebugColor);
		DebugTextBlock->SetFont(FSlateFontInfo(DebugTextBlock->GetFont().FontObject, FontSize));
	}
}


// void UAvengerDebugLogWidget::AvengerLog(FVector2D ScreenLocation, FString DebugContent, AActor* Target, FColor DebugColor,
//                                         float FontSize, float Duration,bool IsRelativePosition)
// {
// 	if (UWorld* World = Target->GetWorld())
// 	{
// 		if(!GetDefault<UAvengerDebugSettings>()->DebugLogClass.IsNull())
// 		{
// 			auto const debugLogClass = GetDefault<UAvengerDebugSettings>()->DebugLogClass;
//
// 			if(auto const logClass = debugLogClass.LoadSynchronous())
// 			{
// 				if(UAvengerDebugLogWidget* DebugWid = CreateWidget<UAvengerDebugLogWidget>(World, logClass))
// 				{
// 					DebugWid->AttachedActor = Target;
// 					DebugWid->ScreenOffset = ScreenLocation;
// 					DebugWid->RelativeOffset = FVector::Zero();
// 					DebugWid->bIsRelativePosition =IsRelativePosition;
// 					DebugWid->bIs2D = true;
// 					DebugWid->SetLocation();
// 					DebugWid->AddToViewport();
// 					DebugWid->bCanTick = true;
// 					DebugWid->TimerDuration = Duration;
// 					DebugWid->bIsTimerActive = true;
// 					
// 					if(UTextBlock* DebugText=DebugWid->DebugTextBlock)
// 					{
// 						const FString Content = Target->GetName() + ": " +DebugContent;
// 						DebugText->SetText(FText::FromString(Content));
// 						DebugText->SetColorAndOpacity(DebugColor);
// 						DebugText->SetFont(FSlateFontInfo(DebugText->GetFont().FontObject, FontSize));
// 					}
// 					
// 				}
// 			}
// 		}
// 	}
// }
//
// void UAvengerDebugLogWidget::AvengerLog(FVector RelativeLocation, FString DebugContent, AActor* Target, FColor DebugColor,
// 	float FontSize, float Duration, bool IsRelativePosition)
// {
// 	if (UWorld* World = Target->GetWorld())
// 	{
// 		if(!GetDefault<UAvengerDebugSettings>()->DebugLogClass.IsNull())
// 		{
// 			auto const debugLogClass = GetDefault<UAvengerDebugSettings>()->DebugLogClass;
//
// 			if(auto const logClass = debugLogClass.LoadSynchronous())
// 			{
// 				if(UAvengerDebugLogWidget* DebugWid = CreateWidget<UAvengerDebugLogWidget>(World, logClass))
// 				{
// 					DebugWid->AttachedActor = Target;
// 					DebugWid->ScreenOffset = FVector2D::Zero();
// 					//DebugWid->CalculateOffset(RelativeLocation);
// 					DebugWid->RelativeOffset = RelativeLocation;
// 					DebugWid->bIsRelativePosition = IsRelativePosition;
// 					DebugWid->bIs2D = false;
// 					DebugWid->SetLocation();
// 					DebugWid->AddToViewport();
// 					DebugWid->bCanTick = true;
// 					DebugWid->TimerDuration = Duration;
// 					DebugWid->bIsTimerActive=true;
// 					
// 					if(UTextBlock* DebugText=DebugWid->DebugTextBlock)
// 					{
// 						const FString Content = Target->GetName() + ": " +DebugContent;
// 						DebugText->SetText(FText::FromString(Content));
// 						DebugText->SetColorAndOpacity(DebugColor);
// 						DebugText->SetFont(FSlateFontInfo(DebugText->GetFont().FontObject, FontSize));
// 					}
// 				}
// 			}
// 		}
// 	}
// }
//
// void UAvengerDebugLogWidget::AvengerLogScreen(FVector2D ScreenLocation, FString DebugContent, AActor* Target, FColor DebugColor,
//                                      float FontSize, float Duration, bool IsRelativePosition)
// {
// 	AvengerLog(ScreenLocation, DebugContent, Target, DebugColor, FontSize, Duration, IsRelativePosition);
// }
//
// void UAvengerDebugLogWidget::AvengerLogWorld(FVector RelativeLocation, FString DebugContent, AActor* Target, FColor DebugColor,
// 	float FontSize, float Duration, bool IsRelativePosition)
// {
// 	AvengerLog(RelativeLocation, DebugContent, Target, DebugColor, FontSize, Duration, IsRelativePosition);
// }

void UAvengerDebugLogWidget::ConfigSetting(FVector RelativeLocation, FString DebugContent, AActor* Target,
	FColor DebugColor, float FontSize, float Duration, bool IsRelativePosition)
{
	AttachedActor = Target;
	ScreenOffset = FVector2D::Zero();
	//DebugWid->CalculateOffset(RelativeLocation);
	RelativeOffset = RelativeLocation;
	bIsRelativePosition = IsRelativePosition;
	bIs2D = false;
	SetLocation();
	// bCanTick = true;
	// TimerDuration = Duration;
	// bIsTimerActive=true;
	DebugLogTicker->SetElapsedTimeMax(Duration);

	if(DebugTextBlock)
	{
		const FString Content = Target->GetName() + ": " +DebugContent;
		DebugTextBlock->SetText(FText::FromString(Content));
		DebugTextBlock->SetColorAndOpacity(DebugColor);
		DebugTextBlock->SetFont(FSlateFontInfo(DebugTextBlock->GetFont().FontObject, FontSize));
	}
}

// void UAvengerDebugLogWidget::CountEnd()
// {
// 	SetVisibility(ESlateVisibility::Hidden); // TODO : どうせ破棄するならVisibility修正する必要はないです。
// 	RemoveFromParent();
// }

void UAvengerDebugLogWidget::SetLocation()
{
	FVector2D ScreenLocation;
	float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
	if(bIsRelativePosition)
	{
		if(bIs2D)
		{
			if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation(), ScreenLocation))
			{
				ScreenLocation=(ScreenLocation+ScreenOffset)/Scale;
				SetPositionInViewport(ScreenLocation, false);
			}
		}
		else if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation()+RelativeOffset, ScreenLocation))
		{
			ScreenLocation /= Scale;
			SetPositionInViewport(ScreenLocation, false);
		}
	}
	else
	{
		if(bIs2D)
		{
			ScreenLocation = ScreenOffset/Scale;
			SetPositionInViewport(ScreenLocation, false);
		}
		else if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), FVector::Zero()+RelativeOffset, ScreenLocation))
		{
			ScreenLocation /= Scale;
			SetPositionInViewport(ScreenLocation, false);
		}
	}
}


void UAvengerDebugLogWidget::CalculateOffset(const FVector &Offset)
{
	FVector2D LocationWithOffset;
	UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation()+Offset, LocationWithOffset);
	FVector2D PivotLocation;
	UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation(), PivotLocation);
	ScreenOffset=LocationWithOffset-PivotLocation;
}