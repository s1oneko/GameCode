// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/AvengerDebugMenu.h"

#include "System/AvengerDefine.h"
#include "AvengerGameplayTag.h"
#include "DebugMenuSubsystem.h"
#include "NativeGameplayTags.h"
#include "Camera/CameraComponent.h"
#include "Character/AvengerPlayerComponent.h"
#include "GameFramework/SpectatorPawn.h"
#include "UI/Common/AvengerUIExtensions.h"
#include "HitBox.h"
#include "Fsm/AvengerFsmComponent.h"
#include "AngActComponent.h"
#include "AvengerDebugExtensions.h"
#include "Action/AvengerActComponent.h"
#include "Action/AvengerActTaskForceFeedback.h"
#include "Character/AvengerCautionComponent.h"
#include "Character/BoostGaugeComponent.h"
#include "Character/HealthComponent.h"
#include "Character/HitStopComponent.h"
#include "Action/AvengerActTaskSetMovementProperty.h"
#include "Destruction/VatComponentBase.h"
#include "Character/TargetSystemComponent.h"
#include "Character/GrappleComponent.h"
#include "Camera/CameraControlComponent.h"
#include "Character/WeaponGaugeComponent.h"
#include "Common/ReactionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
#include "Level/TerritoryActor.h"
#include "Fsm/AvengerFsmEventTriggerConditions.h"
#include "Character/JustDodgeComponent.h"
#include "Gimmick/Gm01_0001/Gm01_0001_BlueprintUtility.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_GAME, "UI.Layer.GameHUD");

void AAvengerDebugMenu::ReceiveInitialized()
{

#if !AVENGER_DEVELOP_ONLY
	AddMenu(FAvengerGameplayTags::Get().DebugTagDefaultTeleportPawn, "Default", "Teleport Pawn Mode", EDebugMenuType::CheckBox, false, 0, false, FKey(),
		FDebugCallback([this](uint8 value)
			{
			if(GetPlayerPawn().IsValid() == false)
				return;
	
			if(value == 0)
			{
				if(DebugTeleportPawn.IsValid())
				{
					PlayerPawn->SetActorLocation(DebugTeleportPawn->GetActorLocation());
					DebugTeleportPawn->K2_DestroyActor();
				}
				
				GetPlayerController()->Possess(PlayerPawn.Get());
				return;
			}

			if(value == 1)
			{
				if(DebugTeleportPawn.IsValid() == false)
					DebugTeleportPawn = GetWorld()->SpawnActor<ASpectatorPawn>(DebugCameraClass, GetPlayerPawn()->GetTransform());
				
				if(auto const debugCameraComp = DebugTeleportPawn->FindComponentByClass<UCameraComponent>())
				{
					if(auto const playerCameraComp = PlayerPawn->FindComponentByClass<UCameraComponent>())
						debugCameraComp->PostProcessSettings = playerCameraComp->PostProcessSettings;
				}

				GetPlayerController()->Possess(DebugTeleportPawn.Get());
			}
		}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagForceFeedback, "Battle", "ForceFeedback Off", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
	FDebugCallback([](uint8 value)
	{
			CVarForceFeedbackSwitch->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagBoostGaugeMax, "Player","Boost Gauge Max",EDebugMenuType::CheckBox,true,0,true,FKey(EName::None),
	FDebugCallback([](uint8 value)
	{
		CVarBoostGaugeMax->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagWeaponGaugeMax, "Player","Weapon Gauge Max",EDebugMenuType::CheckBox,false,0,true,FKey(EName::None),
	FDebugCallback([](uint8 value)
	{
		CVarWeaponGaugeMax->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagForceJustDodgeBonus, "Player", "Force Just Dodge Bonus", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
	FDebugCallback([](uint8 value)
	{
		CVarForceJustDodgeBonus->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagPlayerNoDie, "Player", "Player No Death", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
	FDebugCallback([](uint8 value)
	{
			CVarPlayerNoDeath->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagCameraControlHorizontallyReversed, "Camera", "Horizontal Camera Control", DebugEnum<EDebugCameraControlDirection>(), true, 0, true,
		FDebugCallback([this](uint8 value)
	{
		CVarCameraControlHorizontallyReversed->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagCameraControlVerticallyReversed, "Camera", "Vertical Camera Control", DebugEnum<EDebugCameraControlDirection>(), true, 0, true,
		FDebugCallback([this](uint8 value)
	{
		CVarCameraControlVerticallyReversed->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagCameraControlSensitivity, "Camera", "Camera Control Sensitivity", DebugEnum<EDebugCameraControlSensitivity>(), true, 1, true,
		FDebugCallback([this](uint8 value)
	{
		CVarCameraControlSensitivity->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagDisableChangeCameraParam, "Camera", "Disable Change Camera Param", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
		FDebugCallback([](uint8 value)
	{
		CVarDisableChangeCameraParam->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagEnemyNoDeath, "Enemy", "Enemy No Death", EDebugMenuType::CheckBox, false, 0, true, FKey(EName::None),
		FDebugCallback([](uint8 value)
	{
		CVarEnemyNoDeath->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagQualityPostprocess, "Quality", "PostProcess", DebugEnum<EDebugQuality>(), true, 0, true,
		FDebugCallback([this](uint8 value)
	{
		int quality = 0;

		if (value == (int)EDebugQuality::Default)
		{
			auto settings = GEngine->GameUserSettings;
			quality = settings->GetPostProcessingQuality();
		}
		else 
		{
			quality = value - 1;
		}

		auto pc = UGameplayStatics::GetPlayerController(this, 0);
		FString command = TEXT("sg.PostProcessQuality ");
		command += FString::FromInt(quality);

		pc->ConsoleCommand(command, true);
	}));

#else
	AddMenu(ShowFPSTag, "Default", "Show FPS", EDebugMenuType::CheckBox, true, 0, true, EKeys::One,
		FDebugCallback([this](uint8 value)
		{
			if(IsValid(DebugDefaultWidget))
				DebugDefaultWidget->SetFPSVisibility(value);
		}));
	AddMenu(ShowStatTag, "Default", "Show Stat", EDebugMenuType::CheckBox, true, 0, true, EKeys::Two,
		FDebugCallback([this](uint8 value)
		{
			if(IsValid(DebugDefaultWidget))
				DebugDefaultWidget->SetPerformanceVisibility(value);
		}));
	AddMenu(ShowInputTag, "Default", "Show Input GamePad", EDebugMenuType::CheckBox, true, 0, true, EKeys::Three,
		FDebugCallback([this](uint8 value)
		{
			if(IsValid(DebugDefaultWidget))
				DebugDefaultWidget->SetInputBindVisibility(value);
		}));
	// TODO : DebugCamera
	AddMenu(DebugCameraTag, "Default", "DebugCamera", DebugEnum<EDebugCameraMode>(), true, 0, false,
		FDebugCallback([this](uint8 values)
		{
			if(GetPlayerPawn().IsValid() == false)
				return;
	
			if(values == 0)
			{
				GetPlayerController()->Possess(PlayerPawn.Get());

				if(DebugCameraPawn.IsValid())
					DebugCameraPawn->Destroyed();

				return;
			}
			
			if(values == 1)
			{
				if(DebugCameraPawn.IsValid() == false)
					DebugCameraPawn = GetWorld()->SpawnActor<ASpectatorPawn>(DebugCameraClass, GetPlayerPawn()->GetTransform());
	
				if(auto const debugCameraComp = DebugCameraPawn->FindComponentByClass<UCameraComponent>())
				{
					debugCameraComp->PostProcessSettings = FPostProcessSettings();
				}
	
				GetPlayerController()->Possess(DebugCameraPawn.Get());
			}
			
			if(values == 2)
			{
				if(DebugCameraPawn.IsValid() == false)
					DebugCameraPawn = GetWorld()->SpawnActor<ASpectatorPawn>(DebugCameraClass, GetPlayerPawn()->GetTransform());
				
				if(auto const debugCameraComp = DebugCameraPawn->FindComponentByClass<UCameraComponent>())
				{
					if(auto const playerCameraComp = PlayerPawn->FindComponentByClass<UCameraComponent>())
						debugCameraComp->PostProcessSettings = playerCameraComp->PostProcessSettings;
				}
	
				GetPlayerController()->Possess(DebugCameraPawn.Get());
			}
		}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagDefaultTeleportPawn, "Default", "Teleport Pawn Mode", EDebugMenuType::CheckBox, false, 0, false, FKey(),
		FDebugCallback([this](uint8 value)
		{
			if(GetPlayerPawn().IsValid() == false)
				return;
	
			if(value == 0)
			{
				if(DebugTeleportPawn.IsValid())
				{
					PlayerPawn->SetActorLocation(DebugTeleportPawn->GetActorLocation());
					DebugTeleportPawn->K2_DestroyActor();
				}
				
				GetPlayerController()->Possess(PlayerPawn.Get());
				return;
			}

			if(value == 1)
			{
				if(DebugTeleportPawn.IsValid() == false)
					DebugTeleportPawn = GetWorld()->SpawnActor<ASpectatorPawn>(DebugCameraClass, GetPlayerPawn()->GetTransform());
				
				if(auto const debugCameraComp = DebugTeleportPawn->FindComponentByClass<UCameraComponent>())
				{
					if(auto const playerCameraComp = PlayerPawn->FindComponentByClass<UCameraComponent>())
						debugCameraComp->PostProcessSettings = playerCameraComp->PostProcessSettings;
				}

				GetPlayerController()->Possess(DebugTeleportPawn.Get());
			}
		}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagHitBoxVisualize,"Collision","HitBoxVisualize",DebugEnum<EHitBoxVisualizeType>(),true,0, true,
		FDebugCallback([](uint8 value)
		{
			CVarHitBoxVisualize->Set(value);
		}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagHitBoxGeometryCheckVisualize,"Collision","HitBoxGeometryCheckVisualize",EDebugMenuType::CheckBox,true,0, true,FKey(EName::None),
		FDebugCallback([](uint8 value)
		{
			CVarHitBoxGeometryCheckVisualize->Set(value);
		}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagPushOff, "Collision", "Push Off", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
		FDebugCallback([](uint8 value)
			{
				CVarHitBoxPushOff->Set(value);
			}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagAttackOff, "Collision", "Attack Off", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
		FDebugCallback([](uint8 value)
			{
				CVarHitBoxAttackOff->Set(value);
			}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagHitStopOff, "Collision", "HitStop Off", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
		FDebugCallback([](uint8 value)
			{
				CVarHitStopOff->Set(value);
			}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagLogHitInfo, "Collision", "Log HitInfo", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
		FDebugCallback([](uint8 value)
			{
				CVarLogHitInfo->Set(value);
			}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagVisualizeReaction, "Collision", "VisualizeReaction", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
		FDebugCallback([](uint8 value)
			{
				CVarVisualizeReaction->Set(value);
			}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagShowLaunchAccumulation, "Collision", "Show Launch Accumulation", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
		FDebugCallback([](uint8 value)
			{
				CVarShowLaunchAccumulation->Set(value);
			}));


	AddMenu(FAvengerGameplayTags::Get().DebugTagFsmState, "Battle","Show Fsm State",EDebugMenuType::CheckBox,true,0,true,FKey(EName::None),
		FDebugCallback([](uint8 value)
		{
			CVarVisualizeFsmNames->Set(value);
		}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagActionInfo, "Battle","Show Action Info",EDebugMenuType::CheckBox,true,0,true,FKey(EName::None),
	FDebugCallback([](uint8 value)
	{
		CVarVisualizeActionInfo->Set(value);
	}));
	AddMenu(FAvengerGameplayTags::Get().DebugTagActionInfoMinimal, "Battle", "Show Action Info Minimal", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
		FDebugCallback([](uint8 value)
			{
				CVarVisualizeActionInfoMinimal->Set(value);
			}));
	AddMenu(FAvengerGameplayTags::Get().DebugTagActionInfoLog, "Battle", "Show Action Info Log", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
		FDebugCallback([](uint8 value)
			{
				CVarVisualizeActionInfoLog->Set(value);
			}));
	AddMenu(FAvengerGameplayTags::Get().DebugMovementParameterLog, "Battle", "MovementParameter Log", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
		FDebugCallback([](uint8 value)
			{
				CVarMovementParameterLog->Set(value);
			}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagForceBreakable, "Battle", "Force Breakable", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
		FDebugCallback([](uint8 value)
			{
				CVarForceBreakable->Set(value);
			}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagForceDamageMax, "Battle", "Atk Damage 1000000", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
	FDebugCallback([](uint8 value)
	{
			CVarForceDamageMax->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagForceFeedback, "Battle", "ForceFeedback Off", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
	FDebugCallback([](uint8 value)
	{
			CVarForceFeedbackSwitch->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagVisualDamageLog, "Battle", "Show Damage Log", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
	FDebugCallback([](uint8 value)
	{
			CVarDamageVisualLog->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagVisualizeVelocity, "Player","Show Velocity",EDebugMenuType::CheckBox,true,0,true,FKey(EName::None),
		FDebugCallback([](uint8 value)
		{
			CVarShowPLVelocity->Set(value);
		}));
	
	AddMenu(FAvengerGameplayTags::Get().DebugTagVisualizeBoostGauge, "Player","Show Boost Gauge",EDebugMenuType::CheckBox,true,0,true,FKey(EName::None),
	FDebugCallback([](uint8 value)
	{
		CVarVisualizeBoostGauge->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagBoostGaugeMax, "Player","Boost Gauge Max",EDebugMenuType::CheckBox,true,0,true,FKey(EName::None),
	FDebugCallback([](uint8 value)
	{
		CVarBoostGaugeMax->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagWeaponGaugeMax, "Player","Weapon Gauge Max",EDebugMenuType::CheckBox,false,0,true,FKey(EName::None),
	FDebugCallback([](uint8 value)
	{
		CVarWeaponGaugeMax->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagSoftLockOnTargetDistance, "Player","Show SoftLockOnTargetDistance",EDebugMenuType::CheckBox,true,0,true,FKey(EName::None),
	FDebugCallback([](uint8 value)
	{
		CVarSoftLockOnTargetDistance->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagGrappleTargetDistance, "Player","Show GrappleTargetDistance",EDebugMenuType::CheckBox,true,0,true,FKey(EName::None),
	FDebugCallback([](uint8 value)
	{
		CVarGrappleOnTargetDistance->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagForceJustDodgeBonus, "Player", "Force Just Dodge Bonus", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
	FDebugCallback([](uint8 value)
	{
		CVarForceJustDodgeBonus->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagShowTags, "Player", "Show Tags", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
	FDebugCallback([](uint8 value)
	{
			CVarShowTags->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagPlayerNoDie, "Player", "Player No Death", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
	FDebugCallback([](uint8 value)
	{
			CVarPlayerNoDeath->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagCameraControlHorizontallyReversed, "Camera", "Horizontal Camera Control", DebugEnum<EDebugCameraControlDirection>(), true, 0, true,
		FDebugCallback([this](uint8 value)
	{
		CVarCameraControlHorizontallyReversed->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagCameraControlVerticallyReversed, "Camera", "Vertical Camera Control", DebugEnum<EDebugCameraControlDirection>(), true, 0, true,
		FDebugCallback([this](uint8 value)
	{
		CVarCameraControlVerticallyReversed->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagCameraControlSensitivity, "Camera", "Camera Control Sensitivity", DebugEnum<EDebugCameraControlSensitivity>(), true, 1, true,
		FDebugCallback([this](uint8 value)
	{
		CVarCameraControlSensitivity->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagDisableChangeCameraParam, "Camera", "Disable Change Camera Param", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
		FDebugCallback([](uint8 value)
	{
		CVarDisableChangeCameraParam->Set(value);
	}));
	
	AddMenu(FAvengerGameplayTags::Get().DebugTagVisualizeCameraTarget, "Camera", "Visualize Camera Target", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
		FDebugCallback([](uint8 value)
	{
		CVarVisualizeCameraTarget->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagEnemyNoDeath, "Enemy", "Enemy No Death", EDebugMenuType::CheckBox, false, 0, true, FKey(EName::None),
		FDebugCallback([](uint8 value)
	{
		CVarEnemyNoDeath->Set(value);
	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagShowTerritory, "Enemy", "Show Territory", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
    		FDebugCallback([](uint8 value)
    	{
    		CVarShowTerritory->Set(value);
    	}));

	AddMenu(FAvengerGameplayTags::Get().DebugTagEnemySkill,"Enemy","Enemy Skill", DebugEnum<EDebugEnemySkill>(),true,0, true);

	AddMenu(FAvengerGameplayTags::Get().DebugTagEnemyNoCannonFire, "Enemy", "No Cannon Fire", EDebugMenuType::CheckBox, false, 0, true, FKey(EName::None),
		FDebugCallback([](uint8 value)
			{
				CVarGimmickNoCannonFire->Set(value);
			}));


	AddMenu(FAvengerGameplayTags::Get().DebugTagQualityPostprocess, "Quality", "PostProcess", DebugEnum<EDebugQuality>(), true, 0, true,
		FDebugCallback([this](uint8 value)
	{
		int quality = 0;

		if (value == (int)EDebugQuality::Default)
		{
			auto settings = GEngine->GameUserSettings;
			quality = settings->GetPostProcessingQuality();
		}
		else 
		{
			quality = value - 1;
		}

		auto pc = UGameplayStatics::GetPlayerController(this, 0);
		FString command = TEXT("sg.PostProcessQuality ");
		command += FString::FromInt(quality);

		pc->ConsoleCommand(command, true);
	}));
	
	AddMenu(FAvengerGameplayTags::Get().DebugTagShowDebugLog, "Debug", "Show Debug Log", EDebugMenuType::CheckBox, true, 0, true, FKey(EName::None),
		FDebugCallback([](uint8 value)
		{
				CVarShowDebugLog->Set(value);
		}));

#endif
}


void AAvengerDebugMenu::BeginPlay()
{
	Super::BeginPlay();

	if(!DebugDefaultWidgetClass.IsNull())
	{
		auto const debugDefaultWidgetClass = DebugDefaultWidgetClass.LoadSynchronous();

		if(GetWorld()->GetFirstLocalPlayerFromController())
		{
			DebugDefaultWidget = CreateWidget<UAvengerDebugDefaultWidget>(GetPlayerController()->GetGameInstance(), debugDefaultWidgetClass, FName(TEXT("DefaultDebugWidget")));
			DebugDefaultWidget->AddToViewport(0);
			// DebugDefaultWidget = CastChecked<UAvengerDebugDefaultWidget>(UAvengerUIExtensions::PushContentToLayer_ForPlayer(GetWorld()->GetFirstLocalPlayerFromController(), TAG_UI_LAYER_GAME, debugDefaultWidgetClass));
		}
	}
	
	ReceiveInitialized();
	ReceiveInitializedBP();
}

void AAvengerDebugMenu::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DebugDefaultWidget->Destruct();
	DebugMenuEntries.Reset();
	
	Super::EndPlay(EndPlayReason);
}


TWeakObjectPtr<UDebugMenuSubsystem> AAvengerDebugMenu::GetDebugSubsystem() const
{
	if(GetWorld()->GetGameInstance())
		return GetWorld()->GetGameInstance()->GetSubsystem<UDebugMenuSubsystem>();

	return nullptr;
}

TWeakObjectPtr<APlayerController> AAvengerDebugMenu::GetPlayerController() const
{
	return GetWorld()->GetFirstPlayerController();
}

TWeakObjectPtr<APawn> AAvengerDebugMenu::GetPlayerPawn()
{
	if(PlayerPawn.IsValid() == false)
		PlayerPawn = GetPlayerController()->GetPawn();

	return PlayerPawn;
}

const TArray<UDebugMenuEntry*>& AAvengerDebugMenu::GetDebugMenuEntries() const
{
	return DebugMenuEntries;
}


void AAvengerDebugMenu::AddMenu(FGameplayTag debugTag, FString debugCategory, FString debugLabel, EDebugMenuType debugMenuType, bool bIsUseInitValue, uint8 initValue, bool bIsSaveSetting, FKey shortCutsKey, FDebugCallback onChangeParam, FDebugCallback onUpdateParam)
{
	auto const entryData = NewObject<UDebugMenuEntry>(GetOuter());
	entryData->SetMenuData(debugTag, debugCategory, debugLabel, debugMenuType, bIsUseInitValue, initValue,bIsSaveSetting, nullptr,shortCutsKey, onChangeParam, onUpdateParam);
	DebugMenuEntries.Add(entryData);
}

void AAvengerDebugMenu::AddMenu(FGameplayTag debugTag, FString debugCategory, FString debugLabel, UEnum* enumObject, bool bIsUseInitValue, uint8 initValue, bool bIsSaveSetting, FDebugCallback onChangeParam, FDebugCallback onUpdateParam)
{
	auto const entryData = NewObject<UDebugMenuEntry>(GetOuter());
	entryData->SetMenuData(debugTag, debugCategory, debugLabel, EDebugMenuType::Enum, bIsUseInitValue, initValue, bIsSaveSetting, enumObject, FKey(),onChangeParam, onUpdateParam);
	DebugMenuEntries.Add(entryData);
}

void AAvengerDebugMenu::AddMenuBP(FGameplayTag debugTag, FString debugCategory, FString debugLabel, bool bIsUseInitValue, uint8 initValue, bool bIsSaveSetting, EDebugMenuType debugMenuType, FKey shortCutsKey)
{
	AddMenu(debugTag, debugCategory, debugLabel, debugMenuType, bIsUseInitValue, initValue, bIsSaveSetting, shortCutsKey);
}

void AAvengerDebugMenu::AddMenuBPChange(FGameplayTag debugTag, FString debugCategory, FString debugLabel, bool bIsUseInitValue, uint8 initValue, bool bIsSaveSetting, EDebugMenuType debugMenuType, FKey shortCutsKey, FDebugDynamicDelegate onChangeParam)
{
	AddMenu(debugTag, debugCategory, debugLabel, debugMenuType, bIsUseInitValue, initValue, bIsSaveSetting, shortCutsKey, onChangeParam);
}

void AAvengerDebugMenu::AddMenuBPChangeUpdate(FGameplayTag debugTag, FString debugCategory, FString debugLabel, bool bIsUseInitValue, uint8 initValue, bool bIsSaveSetting, EDebugMenuType debugMenuType, FKey shortCutsKey, FDebugDynamicDelegate onChangeParam, FDebugDynamicDelegate onUpdateParam)
{
	AddMenu(debugTag, debugCategory, debugLabel, debugMenuType, bIsUseInitValue, initValue, bIsSaveSetting, shortCutsKey, onChangeParam, onUpdateParam);
}

void AAvengerDebugMenu::AddMenuBPEnum(FGameplayTag debugTag, FString debugCategory, FString debugLabel, bool bIsUseInitValue, uint8 initValue, bool bIsSaveSetting, UEnum* enumObject)
{
	AddMenu(debugTag, debugCategory, debugLabel, enumObject, bIsUseInitValue, initValue, bIsSaveSetting);	
}

void AAvengerDebugMenu::AddMenuBPEnumChange(FGameplayTag debugTag, FString debugCategory, FString debugLabel, bool bIsUseInitValue, uint8 initValue, bool bIsSaveSetting, UEnum* enumObject, FDebugDynamicDelegate onChangeParam)
{
	AddMenu(debugTag, debugCategory, debugLabel, enumObject, bIsUseInitValue, initValue, bIsSaveSetting, onChangeParam);
}

void AAvengerDebugMenu::AddMenuBPEnumChangeUpdate(FGameplayTag debugTag, FString debugCategory, FString debugLabel, bool bIsUseInitValue, uint8 initValue, bool bIsSaveSetting, UEnum* enumObject, FDebugDynamicDelegate onChangeParam, FDebugDynamicDelegate onUpdateParam)
{
	AddMenu(debugTag, debugCategory, debugLabel, enumObject, bIsUseInitValue, initValue, bIsSaveSetting, onChangeParam, onUpdateParam);
}