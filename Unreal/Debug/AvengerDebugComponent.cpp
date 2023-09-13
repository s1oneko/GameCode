// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/AvengerDebugComponent.h"

// Sets default values for this component's properties
UAvengerDebugComponent::UAvengerDebugComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
#if !UE_BUILD_SHIPPING
	PrimaryComponentTick.bCanEverTick = true;
#else
	PrimaryComponentTick.bCanEverTick = false;
#endif

	// ...
}


// Called when the game starts
void UAvengerDebugComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
#if !UE_BUILD_SHIPPING

	DebugSubsystem = GetOwner()->GetWorld()->GetGameInstance()->GetSubsystem<UDebugMenuSubsystem>();

	if(IsValid(DebugSubsystem))
	{
		TickHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UAvengerDebugComponent::CheckValue), 0.0f);

		for(auto const& tag : DebugTags)
		{
			DebugMaps.Add(tag, DebugSubsystem->GetValue(tag));

			if(OnValueChange.IsBound())
				OnValueChange.Broadcast(tag, DebugSubsystem->GetValue(tag));
		}
	}
#endif
}

void UAvengerDebugComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
#if !UE_BUILD_SHIPPING
	FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
#endif
	
	Super::EndPlay(EndPlayReason);
}


#if !UE_BUILD_SHIPPING
bool UAvengerDebugComponent::CheckValue(float deltaTime)
{
	if(!IsValid(DebugSubsystem))
		return false;

	if(!OnValueChange.IsBound())
		return false;

	for(auto& tuple : DebugMaps)
	{
		auto const debugValue = DebugSubsystem->GetValue(tuple.Key);

		if(debugValue != tuple.Value)
		{
			OnValueChange.Broadcast(tuple.Key, debugValue);
			tuple.Value = debugValue;
		}
	}

	return true;
}
#endif