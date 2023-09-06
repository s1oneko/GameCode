// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugWidget.generated.h"
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDebugLogDelegate);

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

/**
 * 
 */
UCLASS()
class RPG_API UDebugWidget : public UUserWidget
{
	GENERATED_BODY()
	
	// 画面外でもTickを呼ぶために
	UPROPERTY(Transient)
	TObjectPtr<UDebugLogTicker> DebugLogTicker;
	
	bool bIs2D=false;
	
protected:
	
	UFUNCTION()
	void SetLocation();
	UFUNCTION()
	void OnEnd();
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> DebugTextBlock;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UOverlay> DebugOverlay ;
	
	UPROPERTY()
	TObjectPtr<AActor> AttachedActor;
	UPROPERTY()
	FVector WorldLocation;
	UPROPERTY()
	FVector2D ScreenLocation;
	
public:
	
	UDebugWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
	void ConfigSetting(FVector2D screenLocation, FString debugContent,AActor* target, FColor debugColor=FColor::Green,float
		fontSize=20,float duration=5);
	void ConfigSetting(FVector worldLocation, FString debugContent,AActor* target, FColor debugColor=FColor::Green,float
		fontSize=20,float duration=5);
	
};
