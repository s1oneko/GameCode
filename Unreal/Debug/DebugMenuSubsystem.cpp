// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/DebugMenuSubsystem.h"

#include "AvengerDebugSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AvengerLocalPlayer.h"

UDebugMenuSubsystem::UDebugMenuSubsystem()
{
	
}

void UDebugMenuSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	DebugSaveData = Cast<UAvengerDebugSaveGame>(UGameplayStatics::LoadGameFromSlot(DEBUG_SAVE_SLOT, 0));

	if(!DebugSaveData)
	{
		DebugSaveData = Cast<UAvengerDebugSaveGame>(UGameplayStatics::CreateSaveGameObject(UAvengerDebugSaveGame::StaticClass()));

		UGameplayStatics::SaveGameToSlot(DebugSaveData, DEBUG_SAVE_SLOT, 0);
	}

	if(!IsValid(DebugHelper))
	{
		if(!GetDefault<UAvengerDebugSettings>()->DebugHelperClass.IsNull())
		{
			auto const debugHelperClass = GetDefault<UAvengerDebugSettings>()->DebugHelperClass;

			if(auto const helperClass = debugHelperClass.LoadSynchronous())
				DebugHelper = NewObject<UAvengerDebugHelper>(this, helperClass);
		}
	}
	if(!IsValid(DebugLayout))
	{
		if(!GetDefault<UAvengerDebugSettings>()->DebugLayoutClass.IsNull())
		{
			auto const debugLayoutClass = GetDefault<UAvengerDebugSettings>()->DebugLayoutClass;
			if(auto const debugWidget = debugLayoutClass.LoadSynchronous())
				DebugLayout = NewObject<UDebugLayoutWidget>(this, debugWidget);
		}
	}
}

void UDebugMenuSubsystem::Deinitialize()
{
	DebugHelper = nullptr;
	DebugLayout = nullptr;
	Super::Deinitialize();
}

TObjectPtr<UAvengerDebugSaveGame> UDebugMenuSubsystem::LoadDebugData()
{
	if(DebugSaveData)
		return DebugSaveData;

	return nullptr;
}

void UDebugMenuSubsystem::SaveDebugData(FGameplayTag debugTag, uint8 debugValue)
{
	if(IsHasSaveData(debugTag))
		DebugSaveData->DebugData[debugTag] = debugValue;
	
	else
		DebugSaveData->DebugData.Add(debugTag, debugValue);

	UGameplayStatics::SaveGameToSlot(DebugSaveData, DEBUG_SAVE_SLOT, 0);
}

bool UDebugMenuSubsystem::IsHasSaveData(FGameplayTag debugTag)
{
	if(!IsValid(DebugSaveData))
		return false;

	if(DebugSaveData->DebugData.IsEmpty())
		return false;

	return DebugSaveData->DebugData.Contains(debugTag);
}


uint8 UDebugMenuSubsystem::GetValue(FGameplayTag debugTag)
{
	if(IsHasSaveData(debugTag) == false)
		return 0;

	return DebugSaveData->DebugData[debugTag];
}





