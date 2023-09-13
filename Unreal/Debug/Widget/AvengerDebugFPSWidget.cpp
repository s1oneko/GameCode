// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Widget/AvengerDebugFPSWidget.h"

#include "Components/TextBlock.h"

void UAvengerDebugFPSWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if(IsValid(FPSText))
	{
		extern ENGINE_API float GAverageFPS;
		FPSText->SetText(FText::FromString("FPS : " + FString::FromInt(GAverageFPS)));
	}
}