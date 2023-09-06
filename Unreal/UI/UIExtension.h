// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIExtension.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UUIExtension : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UUIExtension() { }
	
	static void DebugLog(FVector2D screenLocation, FString debugContent,AActor* target, FColor debugColor=FColor::Green,float
		fontSize=20,float duration=5);
	
	static void DebugLog(FVector worldLocation, FString debugContent,AActor* target, FColor debugColor=FColor::Green,float
		fontSize=20,float duration=5);

	UFUNCTION(BlueprintCallable,Category="Global Extension")
	static void DebugLogScreen(FVector2D screenLocation, FString debugContent,AActor* target, FColor debugColor=FColor::Green,float
		fontSize=20,float duration=5);
	
	UFUNCTION(BlueprintCallable,Category="Global Extension")
	static void DebugLogWorld(FVector worldLocation, FString debugContent,AActor* target, FColor debugColor=FColor::Green,float
		fontSize=20,float duration=5);
};
