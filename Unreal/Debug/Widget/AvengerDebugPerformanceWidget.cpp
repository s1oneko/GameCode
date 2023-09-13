// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Widget/AvengerDebugPerformanceWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UAvengerDebugPerformanceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// DebugPerformances.Add(FDebugPerformance("Frame", FPSBar, FPSTextBlock));
	DebugPerformances.Add(FDebugPerformance("(CPU)Render", DrawBar, DrawTextBlock));
	DebugPerformances.Add(FDebugPerformance("Game", GameBar, GameTextBlock));
	DebugPerformances.Add(FDebugPerformance("GPU", GPUBar, GPUTextBlock));
	// DebugPerformances.Add(FDebugPerformance("SwapBuffer", SwapBar, SwapTextBlock));
}


void UAvengerDebugPerformanceWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdatePerformanceWidget();
	ChangeWidgetParam();
}


void UAvengerDebugPerformanceWidget::UpdatePerformanceWidget()
{
#if !UE_BUILD_SHIPPING
	if(!GEngine && !GEngine->GameViewport)
		return;

	/** How many cycles the renderthread used (excluding idle time). It's set once per frame in FViewport::Draw. */
	extern RENDERCORE_API uint32 GRenderThreadTime;
	/** How many cycles the gamethread used (excluding idle time). It's set once per frame in FViewport::Draw. */
	extern RENDERCORE_API uint32 GGameThreadTime;
	/** How many cycles it took to swap buffers to present the frame. */
    // extern RENDERCORE_API uint32 GSwapBufferTime;

	const double CurrentTime = FApp::GetCurrentTime();
	const double DeltaTime = CurrentTime - FApp::GetLastTime();

	// Number of milliseconds in total last frame
	const double RawFrameTime = DeltaTime * 1000.0;

	// Number of milliseconds the gamethread was used last frame
	const double RawGameThreadTime = FPlatformTime::ToMilliseconds(GGameThreadTime);

	// Number of milliseconds the renderthread was used last frame
	const double RawRenderThreadTime = FPlatformTime::ToMilliseconds(GRenderThreadTime);

	// Number of milliseconds the GPU was busy last frame
	const uint32 GPUCycles = RHIGetGPUFrameCycles();
	const double RawGPUFrameTime = FPlatformTime::ToMilliseconds(GPUCycles);

	// const double RawSwapBufferTime = FPlatformTime::ToMilliseconds(GSwapBufferTime);

	if(DebugPerformances.Num() > 0)
	{
		// DebugPerformances[0].FrameMs = RawFrameTime;
		DebugPerformances[0].FrameMs = RawRenderThreadTime;
		DebugPerformances[1].FrameMs = RawGameThreadTime;
		DebugPerformances[2].FrameMs = RawGPUFrameTime;
		// DebugPerformances[4].FrameMs = RawSwapBufferTime;
	}

#endif
}

void UAvengerDebugPerformanceWidget::ChangeWidgetParam()
{
	for(auto const& widget : DebugPerformances)
	{
		if(!widget.TextWidget->IsValidLowLevelFast() || !widget.BarWidget->IsValidLowLevelFast())
			continue;

		// widget.TextWidget->SetText(FText::FromString(widget.ThreadName + " : " + FString::SanitizeFloat(widget.FrameMs)));

		auto const panelSlots = Cast<UCanvasPanelSlot>(widget.BarWidget->Slot);
        auto const slotSize = panelSlots->GetSize();
        panelSlots->SetSize(FVector2D(slotSize.X, widget.FrameMs));
		
		auto msValue = widget.FrameMs * 0.01f;
		msValue = FMath::Clamp(msValue, 0.f, 1.f);
		FLinearColor const color(0.f + msValue, 1.f - msValue, 0.f);
		widget.BarWidget->SetColorAndOpacity(color);
	}
}
