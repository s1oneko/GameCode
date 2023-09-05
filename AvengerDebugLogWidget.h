// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ui/Common/AvengerActivatableWidget.h"
#include "AvengerDebugLogWidget.generated.h"

/**
 * 
 */
// Ticker用のDelegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDebugLogDelegate);

// UMGの仕様で画面外にUIが表示されていたらそもそもTickが呼ばれないように作られている。
// なので、UMGのTickはOffにしておいて、別のオブジェクトでTick処理をするように
UCLASS()
class UDebugLogTicker : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

	UDebugLogTicker() { }

public:
	virtual void Tick(float DeltaTime) override
	{
		if(LastFrameNumberWeTicked == GFrameCounter || ElapsedTimeMax == 0.f)
			return;

		if(ElapsedTime > ElapsedTimeMax)
		{
			if(DebugLogOnEndDelegate.IsBound())
				DebugLogOnEndDelegate.Broadcast();

			return;
		}
		
		if(DebugLogOnTickDelegate.IsBound())
			DebugLogOnTickDelegate.Broadcast();

		ElapsedTime += DeltaTime;
		LastFrameNumberWeTicked = GFrameCounter;
	}
	
	virtual ETickableTickType GetTickableTickType() const override
	{
		return ETickableTickType::Always;
	}
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UDebugLogTicker, STATGROUP_Tickables);
	}
	virtual bool IsTickableWhenPaused() const override
	{
		return false;
	}
	virtual bool IsTickableInEditor() const override
	{
		return true;
	}

public:
	UFUNCTION()
	void SetElapsedTimeMax(float elapsedTimeMax) { ElapsedTimeMax = elapsedTimeMax; }

	FDebugLogDelegate& GetDebugLogOnTick() { return DebugLogOnTickDelegate; }
	FDebugLogDelegate& GetDebugOnEnd() { return DebugLogOnEndDelegate; }
	
private:
	uint32 LastFrameNumberWeTicked = INDEX_NONE;

	UPROPERTY(Transient)
	float ElapsedTime = 0.f;

	UPROPERTY(Transient)
	float ElapsedTimeMax = 0.f;

	FDebugLogDelegate DebugLogOnTickDelegate;
	FDebugLogDelegate DebugLogOnEndDelegate;
};

// TODO : 見づらいので、Private-＞Protected-＞Public順番でしてくれるとうれしいです。後関数の方が上で、変数の方が下
// TODO : confluenceにも書いていると思いますが、Local変数は最初小文字で書いてください
// TODO : 後ソースコードの方ももっと短くできそうな部分とか、きれいにかけそうな部分おおいと思うので検討してください。
UCLASS()
class AVENGER_CLIENT_API UAvengerDebugLogWidget : public UAvengerActivatableWidget
{
	GENERATED_BODY()
	
public:
	
	UAvengerDebugLogWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
	void ConfigSetting(FVector2D ScreenLocation, FString DebugContent,AActor* Target, FColor DebugColor=FColor::Green,float
		FontSize=20,float Duration=5, bool IsRelativePosition = false);
	void ConfigSetting(FVector RelativeLocation, FString DebugContent,AActor* Target, FColor DebugColor=FColor::Green,float
		FontSize=20,float Duration=5, bool IsRelativePosition = false);

protected:
	UFUNCTION()
	void OnEnd();
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* DebugTextBlock;

	// TODO : 別にDebug機能だからCanvasPanelとかでもいいですけど、可能であれば一番親はOverlayにして欲しいです。（処理敵にCanvasは重い）
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* DebugCanvas;
	
	// TODO : そもそもいらないでは（生成されたらTickが走るので）
	// UPROPERTY()
	// bool bCanTick=false;

	// TODO : 前にも話しましたが、RelativeLocationで渡すかはAvengerLogを呼ぶ方でパラメーターとして渡すべきで、Widgetで別にやる必要はないです。
	// TODO : WidgetがやることとしてはFVector2DならScreen座標でそのまま表示、FvectorならWorld座標をScreen座標に変換して表示すればいいだけです。
	UPROPERTY()
	bool bIsRelativePosition=false;

	// TODO : 前にも話しましたが、FVector2DかFVectorかによって分岐したいです。現状はフラグがおおい感じがします。
	UPROPERTY()
	bool bIs2D=false;
	
	UPROPERTY()
	AActor* AttachedActor;
	UPROPERTY()
	FVector2D ScreenOffset;
	UPROPERTY()
	FVector RelativeOffset;

	UFUNCTION()
	void SetLocation();

	UFUNCTION()
	void CalculateOffset(const FVector & Offset);

private:
	// 画面外でもTickを呼ぶために
	UPROPERTY(Transient)
	TObjectPtr<UDebugLogTicker> DebugLogTicker;
};