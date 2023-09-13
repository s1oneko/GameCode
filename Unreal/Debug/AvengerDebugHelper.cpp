// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/AvengerDebugHelper.h"

#include "DebugMenuSubsystem.h"
#include "NativeGameplayTags.h"
#include "Player/AvengerLocalPlayer.h"
#include "Widget/AvengerDebugWidget.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MENU, "UI.Layer.Menu");

void UAvengerDebugHelper::ReceiveInitialized()
{
	if(!DebugMenuActorClass.IsNull())
	{
		auto const debugMenuActorClass = DebugMenuActorClass.LoadSynchronous();

		if(GetWorld())
			DebugMenuActor = GetWorld()->SpawnActor<AAvengerDebugMenu>(debugMenuActorClass);
	}

	for (const auto &DebugMenuEntry : DebugMenuActor->GetDebugMenuEntries())
	{
		auto DebugValue = 0;
		
		if(DebugMenuEntry->bIsUseInitValue)
			DebugValue = DebugMenuEntry->InitValue;
		
		if(GetDebugSubsystem()->IsHasSaveData(DebugMenuEntry->DebugTag) && DebugMenuEntry->bIsSaveSetting)
			DebugValue = GetDebugSubsystem()->GetValue(DebugMenuEntry->DebugTag);

		if(DebugMenuEntry->OnChangeParam.IsValid())
		{
			if(DebugMenuEntry->OnChangeParam.Func != nullptr)
				DebugMenuEntry->OnChangeParam.Func(DebugValue);

			DebugMenuEntry->OnChangeParam.DynamicDelegate.ExecuteIfBound(DebugValue);
		}
	}
}

void UAvengerDebugHelper::ResetHelper()
{
	DebugMenuActor->Destroy();
	IsDebugWidgetInitialized = false;
}


void UAvengerDebugHelper::OpenDebugWidget()
{
	if(!DebugMenuClass.IsNull())
	{
		auto const debugMenuClass = DebugMenuClass.LoadSynchronous();
        
		if(GetWorld()->GetFirstLocalPlayerFromController() && IsValid(DebugMenuActor))
		{
			DebugMenuWidget = CastChecked<UAvengerDebugWidget>(UAvengerUIExtensions::PushContentToLayer_ForPlayer(GetWorld()->GetFirstLocalPlayerFromController(), TAG_UI_LAYER_MENU, debugMenuClass));

			if (!IsDebugWidgetInitialized)
			{
				DebugMenuWidget->AddDebugMenu(DebugMenuActor->GetDebugMenuEntries());
				IsDebugWidgetInitialized = true;
			}
			// DebugMenuWidget->DeactivateWidget();
		}
	}
}

UDebugMenuSubsystem* UAvengerDebugHelper::GetDebugSubsystem() const
{
	return CastChecked<UDebugMenuSubsystem>(GetOuter());
}

UWorld* UAvengerDebugHelper::GetWorld() const
{
	return GetDebugSubsystem()->GetWorld();
}