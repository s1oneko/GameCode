// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AvengerDebugHelper.h"
#include "AvengerDebugSaveGame.h"
#include "DebugLayoutWidget.h"
#include "Debug/Widget/AvengerDebugLogWidget.h"
#include "DebugMenuSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class AVENGER_CLIENT_API UDebugMenuSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UDebugMenuSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION()
	void SaveDebugData(FGameplayTag debugTag, uint8 debugValue);

	TObjectPtr<UAvengerDebugSaveGame> LoadDebugData();

	UFUNCTION()
	bool IsHasSaveData(FGameplayTag debugTag);

	UFUNCTION(BlueprintCallable)
	uint8 GetValue(FGameplayTag debugTag);
	
	TObjectPtr<UAvengerDebugHelper> GetDebugHelper() const { return DebugHelper; }

	TObjectPtr<UDebugLayoutWidget> GetDebugLayout() const { return DebugLayout; }
	
	static UDebugLayoutWidget* CreateDebugLayoutWidget(const UWorld* world);
	
private:
	UPROPERTY()
	TMap<UWorld*, UDebugLayoutWidget*> LayoutMap;
	
	const FString DEBUG_SAVE_SLOT = "DebugSaveData";

	UPROPERTY(Transient)
	TObjectPtr<UAvengerDebugSaveGame> DebugSaveData;

	UPROPERTY(Transient)
	TObjectPtr<UAvengerDebugHelper> DebugHelper;

	UPROPERTY(Transient)
	TObjectPtr<UDebugLayoutWidget> DebugLayout;
};