// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/UI/Common/UIExtension.h"

#include "Blueprint/UserWidget.h"
#include "RPG/GlobalSettings.h"
#include "RPG/UI/Widget/DebugWidget.h"

void UUIExtension::DebugLog(FVector2D screenLocation, FString debugContent, AActor* target, FColor debugColor,
                            float fontSize, float duration)
{
	if(target &&target->GetWorld())
	{
		if(!GetDefault<UGlobalSettings>()->DebugWidgetClass.IsNull())
		{
			auto const debugWidgetClass = GetDefault<UGlobalSettings>()->DebugWidgetClass;
			if(auto const debugWidget = debugWidgetClass.LoadSynchronous())
			{
				if(UDebugWidget* DebugWid = CreateWidget<UDebugWidget>(target->GetWorld(),debugWidget))
				{
					DebugWid->ConfigSetting(screenLocation, debugContent, target, debugColor, fontSize, duration);
					DebugWid->AddToViewport();
				}
			}
		}
	}
}

void UUIExtension::DebugLog(FVector worldLocation, FString debugContent, AActor* target, FColor debugColor,
	float fontSize, float duration)
{
	if(target &&target->GetWorld())
	{
		if(!GetDefault<UGlobalSettings>()->DebugWidgetClass.IsNull())
		{
			auto const debugWidgetClass = GetDefault<UGlobalSettings>()->DebugWidgetClass;
			if(auto const debugWidget = debugWidgetClass.LoadSynchronous())
			{
				if(UDebugWidget* DebugWid = CreateWidget<UDebugWidget>(target->GetWorld(),debugWidget))
				{
					DebugWid->ConfigSetting(worldLocation, debugContent, target, debugColor, fontSize, duration);
					DebugWid->AddToViewport();
				}
			}
		}
	}
}

void UUIExtension::DebugLogScreen(FVector2D screenLocation, FString debugContent, AActor* target, FColor debugColor,
	float fontSize, float duration)
{
	DebugLog(screenLocation, debugContent, target, debugColor, fontSize, duration);
}

void UUIExtension::DebugLogWorld(FVector worldLocation, FString debugContent, AActor* target, FColor debugColor,
	float fontSize, float duration)
{
	DebugLog(worldLocation, debugContent, target, debugColor, fontSize, duration);
}
