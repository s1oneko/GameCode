// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Widget/AvengerDebugObjListHoverWidget.h"

#include "Components/TextBlock.h"


void UAvengerDebugObjListHoverWidget::SetHoverInfo(const FDebugUsageMemoryInfo debugUsageMemory) const
{
	if(debugUsageMemory.ObjectRef.IsValid() == false)
		return;

	auto const name = "Name : " + debugUsageMemory.ObjectRef->GetName() + "\n";
	auto const memUsage = "Memory Usage : " + FString::FromInt(debugUsageMemory.ObjectMemoryMax / 1024.f) + " KB\n";
	auto const resUsage = "Resource Memory Usage : " + FString::FromInt(debugUsageMemory.TotalResMemory / 1024.f) + " KB";

	InfoText->SetText(FText::FromString(name + memUsage + resUsage));
}