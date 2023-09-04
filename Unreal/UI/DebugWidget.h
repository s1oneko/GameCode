// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UDebugWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UDebugWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void RPGDebugLog(FVector2D ScreenLocation, FString DebugContent,AActor* Target, FColor DebugColor=FColor::Green,float
		FontSize=20,float Duration=5);
	void RPGDebugLog(FVector RelativeLocation, FString DebugContent,AActor* Target, FColor DebugColor=FColor::Green,float
		FontSize=20,float Duration=5);
	
	UFUNCTION(BlueprintCallable,Category="Debug Log")
	void RPGDebugLogScreen(FVector2D ScreenLocation, FString DebugContent,AActor* Target, FColor DebugColor=FColor::Green,float
		FontSize=20,float Duration=5);
	UFUNCTION(BlueprintCallable,Category="Debug Log")
	void RPGDebugLogWorld(FVector RelativeLocation, FString DebugContent,AActor* Target, FColor DebugColor=FColor::Green,float
		FontSize=20,float Duration=5);

protected:
	
	FTimerHandle HideTimerHandle;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* DebugTextBlock;
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* DebugCanvas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDebugWidget> DebugWidgetBP;
	
	UPROPERTY()
	bool bCanTick=false;
	UPROPERTY()
	AActor* AttachedActor;
	UPROPERTY()
	FVector2D ScreenOffset;
	UPROPERTY()
	FVector RelativeOffset;

	void SetLocation();
	void SetRelativeLocation();
	void CalculateOffset(FVector Offset);
};
