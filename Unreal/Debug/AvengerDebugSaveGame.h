// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "AvengerDebugSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class AVENGER_CLIENT_API UAvengerDebugSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<FGameplayTag, uint8> DebugData;
};