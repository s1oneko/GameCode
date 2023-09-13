// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AvengerDebugSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class AVENGER_CLIENT_API UAvengerDebugSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UAvengerDebugSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){ }

	///** Gets the settings container name for the settings, either Project or Editor */
	virtual FName GetContainerName() const override
	{
		return Super::GetContainerName();
	}
	/** Gets the category for the settings, some high level grouping like, Editor, Engine, Game...etc. */
	virtual FName GetCategoryName() const override
	{
		return "DebugSettings";
	}
	/** The unique name for your section of settings, uses the class's FName. */
	virtual FName GetSectionName() const override
	{
		return Super::GetSectionName();
	}

#if WITH_EDITOR
	/** Gets the section text, uses the classes DisplayName by default. */
	virtual FText GetSectionText() const override
	{
		return FText::FromString("DebugSettings");
	}
	/** Gets the description for the section, uses the classes ToolTip by default. */
	virtual FText GetSectionDescription() const override
	{
		return FText::FromString("DebugSettings");
	}
#endif

public:
	UPROPERTY(Config, EditAnywhere)
	TSoftClassPtr<class UAvengerDebugHelper> DebugHelperClass;
	UPROPERTY(Config, EditAnywhere)
	TSoftClassPtr<class UAvengerDebugLogWidget> DebugLogClass;
	UPROPERTY(Config, EditAnywhere)
	TSoftClassPtr<class UDebugLayoutWidget> DebugLayoutClass;
};
