// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebugMenuSubsystem.h"
#include "Components/ActorComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "AvengerDebugComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDebugValueChange, FGameplayTag, tag, uint8, values);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AVENGER_CLIENT_API UAvengerDebugComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAvengerDebugComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#if !UE_BUILD_SHIPPING
	bool CheckValue(float deltaTime);
#endif
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Debug", meta = (Categories = "SaveDataKey"))
	TArray<FGameplayTag> DebugTags;

	UPROPERTY(VisibleAnywhere)
	TMap<FGameplayTag, uint8> DebugMaps;
	
	UPROPERTY()
	TObjectPtr<UDebugMenuSubsystem> DebugSubsystem;

	UPROPERTY(BlueprintAssignable)
	FOnDebugValueChange OnValueChange;

	FTSTicker::FDelegateHandle TickHandle;
};