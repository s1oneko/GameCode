// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Widget/AvengerDebugWidgetEnumItem.h"

#include "AvengerDebugWidget.h"
#include "Debug/DebugMenuSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UAvengerDebugWidgetEnumItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	OnHovered().AddWeakLambda(this, [this](void)
	{
		if(this->OnChangeHover.IsBound())
			this->OnChangeHover.Broadcast(this);
	});
}


void UAvengerDebugWidgetEnumItem::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	if (IsValid(SoundOnUnhover))
	{
		UGameplayStatics::PlaySound2D(this, SoundOnUnhover);
	}
}


void UAvengerDebugWidgetEnumItem::SetDebugMenuEntryData_Implementation(UDebugMenuEntry* entryData)
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
	
	if(IsValid(DebugMenuEntry->EnumPtr))
	{
		TArray<FText> enumNameArray;

		for(auto i = 0; i < DebugMenuEntry->EnumPtr->NumEnums() - 1; i++)
		{
			auto const label = DebugMenuEntry->Label + " : " + DebugMenuEntry->EnumPtr->GetNameStringByIndex(i);
 			enumNameArray.Add(FText::FromString(label));
		}
		
		PopulateTextLabels(enumNameArray);
		SetSelectedItem(DebugValue);
		OnChangeValue(DebugValue);
	}

	if(DebugMenuEntry->OnUpdateParam.IsValid())
	{
		DebugTicker = NewObject<UDebugTicker>(this);
		DebugTicker->SetCallback(DebugMenuEntry->OnUpdateParam);
	}

	OnCallDelegate();
}

void UAvengerDebugWidgetEnumItem::OnCallDelegate_Implementation()
{
	if(DebugMenuEntry->OnChangeParam.IsValid())
	{
		DebugMenuEntry->OnChangeParam.Func(DebugValue);
		DebugMenuEntry->OnChangeParam.DynamicDelegate.ExecuteIfBound(DebugValue);
	}
}


void UAvengerDebugWidgetEnumItem::OnApplyAction_Implementation()
{
	
}


void UAvengerDebugWidgetEnumItem::OnChangeValue_Implementation(int32 values)
{
	DebugValue = values;
	OnCallDelegate();
	OnSaveDebugValue();
}

void UAvengerDebugWidgetEnumItem::OnChangeNavigate_Implementation()
{
	PlayAnimationForward(Init);
}


void UAvengerDebugWidgetEnumItem::OnSaveDebugValue_Implementation()
{
	if(auto debugSubsystem = GetDebugMenuSubsystem())
		debugSubsystem->SaveDebugData(DebugMenuEntry->DebugTag, DebugValue);
}


UDebugMenuSubsystem* UAvengerDebugWidgetEnumItem::GetDebugMenuSubsystem()
{
	return GetOwningPlayer()->GetWorld()->GetGameInstance()->GetSubsystem<UDebugMenuSubsystem>();
}