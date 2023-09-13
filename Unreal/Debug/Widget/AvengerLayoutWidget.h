// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AvengerLayoutWidget.generated.h"

class UAvengerDebugLogWidget;
/**
 * 
 */
UCLASS()
class AVENGER_CLIENT_API UAvengerLayoutWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	TMap<UActorComponent*, UAvengerDebugLogWidget*> LayoutMap;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UVerticalBox> VerticalBox;
	
public:

	UAvengerLayoutWidget(const FObjectInitializer & ObjectInitializer);
	
	UFUNCTION()
	void RegisterOverlay(UActorComponent* component, UAvengerDebugLogWidget* overlay);

	UFUNCTION()
	UAvengerDebugLogWidget* GetOverlay(UActorComponent* component);

	UFUNCTION()
	void RemoveFromMap(UActorComponent* component);
	
	UFUNCTION()
	void SetLocation(FVector Location);
};
