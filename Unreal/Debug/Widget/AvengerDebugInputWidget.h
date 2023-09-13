// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "AvengerDebugInputWidget.generated.h"

// TODO : APlayerController見たけど、よさそうなやつがなかったが、EnhancedInputでどうにかできないのか確認
UENUM()
enum class EDebugPadInput : uint32
{
	None = 0,
	LeftThumbstick = 1 << 1,
	RightThumbstick = 1 << 2,
	SpecialLeft = 1 << 3,
	// SpecialLeft_X = 1 << 4,
	// SpecialLeft_Y = 1 << 5,
	SpecialRight = 1 << 6,
	FaceButton_Bottom = 1 << 7,
	FaceButton_Right = 1 << 8,
	FaceButton_Left = 1 << 9,
	FaceButton_Top = 1 << 10,
	LeftShoulder = 1 << 11,
	RightShoulder = 1 << 12,
	LeftTrigger = 1 << 13,
	RightTrigger = 1 << 14,
	Up = 1 << 15,
	Down = 1 << 16,
	Right = 1 << 17,
	Left = 1 << 18,
	BTN_R_D = FaceButton_Bottom,
	BTN_R_R = FaceButton_Right,
	BTN_R_U = FaceButton_Top,
	BTN_R_L = FaceButton_Left,
	BTN_L_D = Down,
	BTN_L_R = Right,
	BTN_L_U = Up,
	BTN_L_L = Left
};

USTRUCT()
struct FDebugPadInputInfo
{
	GENERATED_BODY()

	FDebugPadInputInfo()
	{
		Key = FKey();
		Bit = 0;
	}
	
	FDebugPadInputInfo(FKey key, uint32 bit) : Key(key), Bit(bit) {}

	UPROPERTY(Transient)
	FKey Key;

	UPROPERTY(Transient)
	uint32 Bit;
};

USTRUCT()
struct FDebugPadBtnImageInfo
{
	GENERATED_BODY()

	FDebugPadBtnImageInfo()
	{
		Key = EDebugPadInput::None;
		BtnImage = nullptr;
	}
	
	FDebugPadBtnImageInfo(EDebugPadInput key, class UImage* btnImage) : Key(key), BtnImage(btnImage) {}

	UPROPERTY(Transient)
	EDebugPadInput Key;

	UPROPERTY(Transient)
	class UImage* BtnImage;
};

/**
 * 
 */
UCLASS()
class AVENGER_CLIENT_API UAvengerDebugInputWidget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	uint32 GetInputInfo() const;
	bool IsPressedBtn(EDebugPadInput debugBtn) const;
	void MoveLStickPanel(FVector2D vec) const;
	void MoveRStickPanel(FVector2D vec) const;
	
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> AButtonImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> BButtonImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> XButtonImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> YButtonImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> DPadUpImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> DPadDownImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> DPadRightImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> DPadLeftImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> StartButtonImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> SelectButtonImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> SharedButtonImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> LButtonImage;
	
	UPROPERTY(meta= (BindWidgetOptional))
	TObjectPtr<class UImage> LTriggerImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> RButtonImage;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UImage> RTriggerImage;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UCanvasPanel> LStickPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UCanvasPanel> RStickPanel;

	UPROPERTY(Transient)
	TWeakObjectPtr<class UCanvasPanelSlot> LStickPanelSlot;

	UPROPERTY(Transient)
	TWeakObjectPtr<class UCanvasPanelSlot> RStickPanelSlot;

	UPROPERTY(Transient)
	FVector2D LStickPanelPos;

	UPROPERTY(Transient)
	FVector2D RStickPanelPos;

	UPROPERTY(Transient)
	TWeakObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(Transient)
	TArray<FDebugPadBtnImageInfo> DebugBtnImages;
	
	const TArray<FDebugPadInputInfo> DEBUG_INPUT_INFO =
		{
		FDebugPadInputInfo(EKeys::Gamepad_LeftThumbstick, static_cast<uint32>(EDebugPadInput::LeftThumbstick)),
		FDebugPadInputInfo(EKeys::Gamepad_RightThumbstick, static_cast<uint32>(EDebugPadInput::RightThumbstick)),
		FDebugPadInputInfo(EKeys::Gamepad_Special_Left, static_cast<uint32>(EDebugPadInput::SpecialLeft)),
		// FDebugPadInputInfo(EKeys::Gamepad_Special_Left_X, static_cast<uint32>(EDebugPadInput::SpecialLeft_X)),
		// FDebugPadInputInfo(EKeys::Gamepad_Special_Left_Y, static_cast<uint32>(EDebugPadInput::SpecialLeft_Y)),
		FDebugPadInputInfo(EKeys::Gamepad_Special_Right, static_cast<uint32>(EDebugPadInput::SpecialRight)),
		FDebugPadInputInfo(EKeys::Gamepad_LeftShoulder, static_cast<uint32>(EDebugPadInput::LeftShoulder)),
		FDebugPadInputInfo(EKeys::Gamepad_RightShoulder, static_cast<uint32>(EDebugPadInput::RightShoulder)),
		FDebugPadInputInfo(EKeys::Gamepad_LeftTrigger, static_cast<uint32>(EDebugPadInput::LeftTrigger)),
		FDebugPadInputInfo(EKeys::Gamepad_RightTrigger, static_cast<uint32>(EDebugPadInput::RightTrigger)),
		FDebugPadInputInfo(EKeys::Gamepad_FaceButton_Bottom, static_cast<uint32>(EDebugPadInput::BTN_R_D)),
		FDebugPadInputInfo(EKeys::Gamepad_FaceButton_Right, static_cast<uint32>(EDebugPadInput::BTN_R_R)),
		FDebugPadInputInfo(EKeys::Gamepad_FaceButton_Left, static_cast<uint32>(EDebugPadInput::BTN_R_L)),
		FDebugPadInputInfo(EKeys::Gamepad_FaceButton_Top, static_cast<uint32>(EDebugPadInput::BTN_R_U)),
		FDebugPadInputInfo(EKeys::Gamepad_DPad_Up, static_cast<uint32>(EDebugPadInput::BTN_L_U)),
		FDebugPadInputInfo(EKeys::Gamepad_DPad_Down, static_cast<uint32>(EDebugPadInput::BTN_L_D)),
		FDebugPadInputInfo(EKeys::Gamepad_DPad_Right, static_cast<uint32>(EDebugPadInput::BTN_L_R)),
		FDebugPadInputInfo(EKeys::Gamepad_DPad_Left, static_cast<uint32>(EDebugPadInput::BTN_L_L)),
		FDebugPadInputInfo(EKeys::AnyKey, 0),
		};
	// 現フレームでの入力
	uint32 BtnOn;
	uint32 BtnPressed;
	uint32 BtnRelease;

	// joystick R
	float JoyRd;
	float JoyRX;
	float JoyRY;

	// joystick L
	float JoyLd;
	float JoyLX;
	float JoyLY;
};
