// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AvengerDebugMenu.h"
#include "Widget/AvengerDebugDefaultWidget.h"
#include "AvengerDebugHelper.generated.h"

// WidgetのListView用のデータ, Structにしたいけど、ListviewだとUObjectじゃないといけない…？
UCLASS(BlueprintType)
class UDebugMenuEntry : public UObject
{
	GENERATED_BODY()

public:
	UDebugMenuEntry() {}

	void SetMenuData(const FGameplayTag debugTag, const FString category, const FString label,
		EDebugMenuType menuType, bool bisUseInitValue, uint8 initValue, bool bisSaveSetting, UEnum* enumPtr = nullptr, const FKey shortsCutsKey = FKey(),
		FDebugCallback onChangeParam = FDebugCallback(), FDebugCallback onUpdateParam = FDebugCallback())
	{
		DebugTag = debugTag;
		Category = category;
		Label = label;
		MenuType = menuType;
		bIsUseInitValue = bisUseInitValue;
		InitValue = initValue;
		bIsSaveSetting = bisSaveSetting;
		EnumPtr = enumPtr;
		ShortsCutKey = shortsCutsKey;
		OnChangeParam = onChangeParam;
		OnUpdateParam = onUpdateParam;
	}

	UPROPERTY(BlueprintReadOnly, Transient)
	FGameplayTag DebugTag;

	UPROPERTY(BlueprintReadOnly, Transient)
	FString Category;

	UPROPERTY(BlueprintReadOnly, Transient)
	FString Label;

	UPROPERTY(BlueprintReadOnly, Transient)
	EDebugMenuType MenuType;

	UPROPERTY(BlueprintReadOnly, Transient)
	bool bIsUseInitValue;

	UPROPERTY(BlueprintReadOnly, Transient)
	uint8 InitValue;

	UPROPERTY(BlueprintReadOnly, Transient)
	bool bIsSaveSetting;

	UPROPERTY(BlueprintReadOnly, Transient)
	UEnum* EnumPtr;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	FKey ShortsCutKey;

	FDebugCallback OnChangeParam;
	FDebugCallback OnUpdateParam;
};

/**
 * 
 */
UCLASS(Blueprintable, Config = Game, Within = DebugMenuSubsystem)
class AVENGER_CLIENT_API UAvengerDebugHelper : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ReceiveInitialized();

	UFUNCTION()
	void ResetHelper();

	UFUNCTION()
	void OpenDebugWidget();
	
	UDebugMenuSubsystem* GetDebugSubsystem() const;
	
	virtual UWorld* GetWorld() const override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<class UAvengerDebugWidget> DebugMenuClass;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<class AAvengerDebugMenu> DebugMenuActorClass;

	UPROPERTY(Transient)
	TObjectPtr<UAvengerDebugWidget> DebugMenuWidget;

	UPROPERTY(Transient)
	TObjectPtr<class AAvengerDebugMenu> DebugMenuActor;

	UPROPERTY(Transient)
	bool IsDebugWidgetInitialized = false;
	
	friend class UDebugMenuSubsystem;
};