// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/AvengerDebugExtensions.h"

#include "AvengerDebugSettings.h"
#include "AvengerGameplayTag.h"
#include "DebugMenuSubsystem.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Debug/Widget/AvengerDebugLogWidget.h"

void UAvengerDebugExtensions::AvengerLog(const FVector2D& screenLocation, const FString& debugContent,  AActor* actor,
	FColor debugColor, float fontSize, float duration)
{
	if(actor)
	{
		if(auto const debugSubSystem = actor->GetWorld()->GetGameInstance()->GetSubsystem<UDebugMenuSubsystem>())
		{
			auto world =actor->GetWorld();
			UDebugLayoutWidget* layout = debugSubSystem->GetDebugLayout();
			
			UAvengerDebugLogWidget* overlay;
			overlay = layout->GetText();
			overlay->ConfigSetting(screenLocation, debugContent, actor, debugColor,fontSize, duration);
		}
	}
}

void UAvengerDebugExtensions::AvengerLog(const FVector& worldLocation, const FString& debugContent, AActor* actor,
	FColor debugColor, float fontSize, float duration)
{
	if(actor)
	{
		if(auto const debugSubSystem = actor->GetWorld()->GetGameInstance()->GetSubsystem<UDebugMenuSubsystem>())
		{
			UDebugLayoutWidget* layout = debugSubSystem->GetDebugLayout();
			
			UAvengerDebugLogWidget* overlay;
			
			overlay = layout->GetText();
			overlay->ConfigSetting(worldLocation, debugContent, actor, debugColor,fontSize, duration);
		}
	}
}

void UAvengerDebugExtensions::AvengerLogScreenBP(const FVector2D& screenLocation, const FString& debugContent, AActor* actor,
	FColor debugColor, float fontSize, float duration)
{
	AvengerLog(screenLocation,debugContent, actor, debugColor, fontSize,duration);
}

void UAvengerDebugExtensions::AvengerLogWorldBP(const FVector& worldLocation, const FString& debugContent, AActor* actor,
	FColor debugColor, float fontSize, float duration)
{
	AvengerLog( worldLocation,debugContent, actor, debugColor, fontSize,duration);
}





