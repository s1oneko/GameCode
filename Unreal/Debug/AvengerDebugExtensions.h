// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AvengerDebugExtensions.generated.h"

/**
 * 
 */
static TAutoConsoleVariable<int32> CVarShowDebugLog(
		TEXT("Avenger.Debug.ShowLog"),
		0,
		TEXT("デバッグの表示非表示を切り替える\n")
		TEXT("<=1: ON\n")
		TEXT("  0: OFF\n"),
		ECVF_Scalability | ECVF_RenderThreadSafe);

UCLASS()
class AVENGER_CLIENT_API UAvengerDebugExtensions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UAvengerDebugExtensions() { }
	
	static class UAvengerDebugLogWidget* CreateDebugLogWidget(const UActorComponent* outer);
	
	static void AvengerLog(const FVector2D& screenLocation, const FString& debugContent, AActor* actor,
		FColor debugColor = FColor::Green, float fontSize = 20.f, float duration = 5.f);
	
	static void AvengerLog(const FVector& worldLocation, const FString& debugContent, AActor* actor,
		FColor debugColor = FColor::Green, float fontSize=20.f, float duration=5.f);

	UFUNCTION(BlueprintCallable, Category = "Global Debug Extensions", DisplayName = "Avenger Log(Screen)")
	static void AvengerLogScreenBP(const FVector2D& screenLocation, const FString& debugContent, AActor* actor,
		FColor debugColor=FColor::Green, float fontSize = 20.f, float duration = 5.f);
	
	UFUNCTION(BlueprintCallable, Category = "Global Debug Extensions", DisplayName = "Avenger Log(World)")
	static void AvengerLogWorldBP(const FVector& worldLocation, const FString& debugContent, AActor* actor,
		FColor debugColor = FColor::Green, float fontSize = 20.f, float duration = 5.f);
};
