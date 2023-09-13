// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AvengerDebugMenu.generated.h"

class UDebugMenuSubsystem;
class UDebugMenuEntry;

DECLARE_DELEGATE_OneParam(FDebugDelegate, uint8);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDebugDynamicDelegate, uint8, values);

template <typename DebugEnumType>
static UEnum* DebugEnum()
{
	return StaticEnum<DebugEnumType>();
}

UENUM(BlueprintType)
enum class EDebugMenuType : uint8
{
	None,
	Button,
	CheckBox,
	Enum,
};

UENUM()
enum class EDebugCameraMode : uint8
{
	Off,
	On_NotPostProcessCamera,
	On_PostProcessCamera,
};

UENUM()
enum class EDebugCameraControlDirection: uint8
{
	Normal,
	Reversed,
};

UENUM()
enum class EDebugCameraControlSensitivity: uint8
{
	Low,
	Middle,
	High,
};

UENUM()
enum class EDebugQuality: uint8
{
	Default,
	Low,
	Medium,
	High,
	Epic,
	Cinematic
};

UENUM(BlueprintType)
enum class EDebugEnemySkill : uint8
{
	Skill_0000 = 0,
	Skill_0001 = 1,
	Skill_0002 = 2,
	Skill_0003 = 3,
	SKill_0004 = 4,
	Skill_0005 = 5,
};

USTRUCT(BlueprintType)
struct FDebugCallback
{
	GENERATED_BODY()

	FDebugCallback() {}
	FDebugCallback(const TFunction<void(uint8)> &func) : Func(func) {}
	FDebugCallback(const FDebugDynamicDelegate &dynamicDelegate) : DynamicDelegate(dynamicDelegate) {}

	bool IsValid() const
	{
		return Func || DynamicDelegate.IsBound();
	}

	TFunction<void(uint8)> Func;
	FDebugDynamicDelegate DynamicDelegate;
};

/**
 * 
 */
UCLASS(Blueprintable, MinimalAPI, meta = (ShowWorldContextPin))
class AAvengerDebugMenu : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintCallable)
	void ReceiveInitialized();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Receive Initialized")
	void ReceiveInitializedBP();

	void AddMenu(FGameplayTag debugTag, FString debugCategory, FString debugLabel, EDebugMenuType debugMenuType,
		bool bIsUseInitValue = false, uint8 initValue = 0, bool bIsSaveSetting = true, FKey shortCutsKey = FKey(EName::None),
		FDebugCallback onChangeParam = FDebugCallback(), FDebugCallback onUpdateParam = FDebugCallback());
	
	// enum用、別に１個でもいいが、ちょっと使いやすくするため
	void AddMenu(FGameplayTag debugTag, FString debugCategory, FString debugLabel, UEnum* enumObject, bool bIsUseInitValue = false, uint8 initValue = 0,
		bool bIsSaveSetting = true, FDebugCallback onChangeParam = FDebugCallback(), FDebugCallback onUpdateParam = FDebugCallback());

	// BP用
	UFUNCTION(BlueprintCallable, DisplayName = "Add Debug Menu(Not Callback)")
	void AddMenuBP(FGameplayTag debugTag, FString debugCategory, FString debugLabel, bool bIsUseInitValue, uint8 initValue, bool bIsSaveSetting, EDebugMenuType debugMenuType, FKey shortCutsKey);
	
	UFUNCTION(BlueprintCallable, DisplayName = "Add Debug Menu(Change)")
	void AddMenuBPChange(FGameplayTag debugTag, FString debugCategory, FString debugLabel, bool bIsUseInitValue, uint8 initValue, bool bIsSaveSetting, EDebugMenuType debugMenuType,
		FKey shortCutsKey, FDebugDynamicDelegate onChangeParam);
	
	UFUNCTION(BlueprintCallable, DisplayName = "Add Debug Menu(Change, Update)")
	void AddMenuBPChangeUpdate(FGameplayTag debugTag, FString debugCategory, FString debugLabel, bool bIsUseInitValue, uint8 initValue, bool bIsSaveSetting, EDebugMenuType debugMenuType,
		FKey shortCutsKey, FDebugDynamicDelegate onChangeParam, FDebugDynamicDelegate onUpdateParam);

	UFUNCTION(BlueprintCallable, DisplayName = "Add Debug Menu(Enum, Not Callback)")
	void AddMenuBPEnum(FGameplayTag debugTag, FString debugCategory, FString debugLabel, bool bIsUseInitValue, uint8 initValue, bool bIsSaveSetting, UEnum* enumObject);

	UFUNCTION(BlueprintCallable, DisplayName = "Add Debug Menu(Enum, Change)")
	void AddMenuBPEnumChange(FGameplayTag debugTag, FString debugCategory, FString debugLabel, bool bIsUseInitValue, uint8 initValue, bool bIsSaveSetting, UEnum* enumObject,
		FDebugDynamicDelegate onChangeParam);

	UFUNCTION(BlueprintCallable, DisplayName = "Add Debug Menu(Enum, Change, Update)")
	void AddMenuBPEnumChangeUpdate(FGameplayTag debugTag, FString debugCategory, FString debugLabel, bool bIsUseInitValue, uint8 initValue, bool bIsSaveSetting, UEnum* enumObject,
		FDebugDynamicDelegate onChangeParam, FDebugDynamicDelegate onUpdateParam);
	
	// ↓からはDefault用
	TWeakObjectPtr<class UDebugMenuSubsystem> GetDebugSubsystem() const;
	TWeakObjectPtr<APlayerController> GetPlayerController() const;
	TWeakObjectPtr<class APawn> GetPlayerPawn();

public:
	const TArray<UDebugMenuEntry*>& GetDebugMenuEntries() const;

private:
	UPROPERTY(Transient)
	TArray<UDebugMenuEntry*> DebugMenuEntries;
	
	// Default用
	UPROPERTY(EditDefaultsOnly, Category = "DefaultDebug")
	TSoftClassPtr<class UAvengerDebugDefaultWidget> DebugDefaultWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "DefaultDebug")
	TSubclassOf<APawn> DebugCameraClass;

	UPROPERTY(EditDefaultsOnly, Category = "DefaultDebug")
	FGameplayTag ShowFPSTag;

	UPROPERTY(EditDefaultsOnly, Category = "DefaultDebug")
	FGameplayTag ShowInputTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "DefaultDebug")
	FGameplayTag ShowStatTag;

	UPROPERTY(EditDefaultsOnly, Category = "DefaultDebug")
	FGameplayTag DebugCameraTag;
	
	UPROPERTY(Transient)
	TObjectPtr<UAvengerDebugDefaultWidget> DebugDefaultWidget;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<APawn> DebugCameraPawn;

	UPROPERTY(Transient)
	TWeakObjectPtr<APawn> DebugTeleportPawn;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<APawn> PlayerPawn;
};