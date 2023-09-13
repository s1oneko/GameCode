// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Widget/AvengerDebugWidgetItem.h"

#include "AvengerDebugWidget.h"
#include "Debug/DebugMenuSubsystem.h"
#include "Debug/AvengerDebugHelper.h"

void UAvengerDebugWidgetItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OnHovered().AddWeakLambda(this, [this](void)
	{
		if(this->OnChangeHover.IsBound())
			this->OnChangeHover.Broadcast(this);
	});
}

void UAvengerDebugWidgetItem::SetDebugMenuEntryData_Implementation(UDebugMenuEntry* entryData)
{
	if(!IsValid(entryData))
		return;

	DebugMenuEntry = entryData;

	DebugValue = 0;
	
	if(DebugMenuEntry->bIsUseInitValue)
		DebugValue = DebugMenuEntry->InitValue;
	
	if(GetDebugMenuSubsystem()->IsHasSaveData(DebugMenuEntry->DebugTag) && DebugMenuEntry->bIsSaveSetting)
		DebugValue = GetDebugMenuSubsystem()->GetValue(DebugMenuEntry->DebugTag);
	
	OnSaveDebugValue();
	
	if(entryData->ShortsCutKey.IsValid())
	{
		if(auto const inputComponent = GetOwningPlayer()->InputComponent)
			inputComponent->BindKey(entryData->ShortsCutKey, IE_Pressed, this, &UAvengerDebugWidgetItem::OnApplyAction_Implementation);
	}

	if(DebugMenuEntry->OnUpdateParam.IsValid())
	{
		DebugTicker = NewObject<UDebugTicker>(this);
		DebugTicker->SetCallback(DebugMenuEntry->OnUpdateParam);
	}

	if(OnFinishEntryData.IsBound())
		OnFinishEntryData.Broadcast(entryData);

	OnCallDelegate();
}

void UAvengerDebugWidgetItem::OnCallDelegate_Implementation()
{
	if(!DebugMenuEntry.IsValid())
		return;
	
	if(DebugMenuEntry->OnChangeParam.IsValid())
	{
		if(DebugMenuEntry->OnChangeParam.Func != nullptr)
			DebugMenuEntry->OnChangeParam.Func(DebugValue);

		DebugMenuEntry->OnChangeParam.DynamicDelegate.ExecuteIfBound(DebugValue);
	}

	if(OnChangeParam.IsBound())
		OnChangeParam.Broadcast(DebugValue);
}


void UAvengerDebugWidgetItem::OnApplyAction_Implementation()
{
	DebugValue = !DebugValue;
	OnCallDelegate();
	OnSaveDebugValue();
}


void UAvengerDebugWidgetItem::OnChangeValue_Implementation(int32 values)
{
	
}

void UAvengerDebugWidgetItem::OnChangeNavigate_Implementation()
{
	PlayAnimationForward(Init);
}

void UAvengerDebugWidgetItem::OnSaveDebugValue_Implementation()
{
	if(!DebugMenuEntry.IsValid())
		return;
		
	if(const auto debugSubsystem = GetDebugMenuSubsystem())
		debugSubsystem->SaveDebugData(DebugMenuEntry->DebugTag, DebugValue);
}


UDebugMenuSubsystem* UAvengerDebugWidgetItem::GetDebugMenuSubsystem()
{
	return GetOwningPlayer()->GetWorld()->GetGameInstance()->GetSubsystem<UDebugMenuSubsystem>();
}