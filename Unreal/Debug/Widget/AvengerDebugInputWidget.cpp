// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Widget/AvengerDebugInputWidget.h"

#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"


void UAvengerDebugInputWidget::NativeOnInitialized()
{
	BtnOn = BtnPressed = BtnRelease = 0;
	JoyRd = JoyRX = JoyRY = 0.f;
	JoyLd = JoyLX = JoyLY = 0.f;

	if(IsValid(LStickPanel))
	{
		LStickPanelSlot = Cast<UCanvasPanelSlot>(LStickPanel->Slot);
		LStickPanelPos = LStickPanelSlot->GetPosition();
	}

	if(IsValid(RStickPanel))
	{
		RStickPanelSlot = Cast<UCanvasPanelSlot>(RStickPanel->Slot);
		RStickPanelPos = RStickPanelSlot->GetPosition();
	}

	// TODO : PlayerController取得
	PlayerController = GetWorld()->GetFirstPlayerController();
	
	// TODO : bindうまくできないのか確認
	DebugBtnImages.Add(FDebugPadBtnImageInfo(EDebugPadInput::SpecialLeft, SelectButtonImage.Get()));
	DebugBtnImages.Add(FDebugPadBtnImageInfo(EDebugPadInput::SpecialRight, StartButtonImage.Get()));
	DebugBtnImages.Add(FDebugPadBtnImageInfo(EDebugPadInput::LeftShoulder, LButtonImage.Get()));
	DebugBtnImages.Add(FDebugPadBtnImageInfo(EDebugPadInput::RightShoulder, RButtonImage.Get()));
	DebugBtnImages.Add(FDebugPadBtnImageInfo(EDebugPadInput::LeftTrigger, LTriggerImage.Get()));
	DebugBtnImages.Add(FDebugPadBtnImageInfo(EDebugPadInput::RightTrigger, RTriggerImage.Get()));
	DebugBtnImages.Add(FDebugPadBtnImageInfo(EDebugPadInput::FaceButton_Bottom, AButtonImage.Get()));
	DebugBtnImages.Add(FDebugPadBtnImageInfo(EDebugPadInput::FaceButton_Right, BButtonImage.Get()));
	DebugBtnImages.Add(FDebugPadBtnImageInfo(EDebugPadInput::FaceButton_Left, XButtonImage.Get()));
	DebugBtnImages.Add(FDebugPadBtnImageInfo(EDebugPadInput::FaceButton_Top, YButtonImage.Get()));
	DebugBtnImages.Add(FDebugPadBtnImageInfo(EDebugPadInput::Up, DPadUpImage.Get()));
	DebugBtnImages.Add(FDebugPadBtnImageInfo(EDebugPadInput::Down, DPadDownImage.Get()));
	DebugBtnImages.Add(FDebugPadBtnImageInfo(EDebugPadInput::Right, DPadRightImage.Get()));
	DebugBtnImages.Add(FDebugPadBtnImageInfo(EDebugPadInput::Left, DPadLeftImage.Get()));
}


void UAvengerDebugInputWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if(PlayerController != nullptr)
	{
		// button
		auto const btn = GetInputInfo();
		BtnOn = btn & (btn ^ BtnPressed);
		BtnRelease = (~btn) & BtnPressed;
		BtnPressed = btn;

		for(auto const& inputMap : DebugBtnImages)
		{
			if(IsValid(inputMap.BtnImage))
				inputMap.BtnImage->SetVisibility(IsPressedBtn(inputMap.Key) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
		
		// joystick
		FVector vec;

		// left
		PlayerController->GetInputAnalogStickState(EControllerAnalogStick::CAS_LeftStick, JoyLX, JoyLY);
		JoyLY *= -1; // なぜか逆になってる？
		vec.Z = 0;
		vec.X = JoyLX;
		vec.Y = JoyLY;
		JoyLd = vec.Distance(vec, FVector::ZeroVector);

		if(JoyLd == 0)
		{
			if(const uint32 btnInfo = GetInputInfo())
			{
				JoyLd = 1;
				if(btnInfo & 0x1) JoyLY = 1;
				if(btnInfo & 0x2) JoyLY = -1;
				if(btnInfo & 0x4) JoyLX = 1;
				if(btnInfo & 0x8) JoyLX = -1;
			}
		}

		// right
		PlayerController->GetInputAnalogStickState(EControllerAnalogStick::CAS_RightStick, JoyRX, JoyRY);
		vec.X = JoyRX;
		vec.Y = JoyRY;
		JoyRd = vec.Distance(vec, FVector::ZeroVector);

		if(JoyRd == 0)
		{
			if(uint32 const btnInfo = GetInputInfo())
			{
				JoyRd = 1;
				if(btnInfo & 0x1) JoyRY = 1;
				if(btnInfo & 0x2) JoyRY = -1;
				if(btnInfo & 0x4) JoyRX = 1;
				if(btnInfo & 0x8) JoyRX = -1;
			}
		}

		// なぜかSpecial Left Buttonを押すとAnalogStickの方もBindingされてしまうので固定
		if(IsPressedBtn(EDebugPadInput::SpecialLeft))
			JoyLX = JoyRX = 0;	
		
		// Stickのイメージ位置
		if(IsValid(LStickPanel))
			MoveLStickPanel(FVector2D(JoyLX, JoyLY));

		if(IsValid(RStickPanel))
			MoveRStickPanel(FVector2D(JoyRX, JoyRY));
	}
}

void UAvengerDebugInputWidget::MoveLStickPanel(FVector2D vec) const
{
	LStickPanelSlot->SetPosition(LStickPanelPos + (vec * 10.f));
}

void UAvengerDebugInputWidget::MoveRStickPanel(FVector2D vec) const
{
	RStickPanelSlot->SetPosition(RStickPanelPos + (vec * 10.f));
}


bool UAvengerDebugInputWidget::IsPressedBtn(EDebugPadInput debugBtn) const
{
	return (BtnPressed & static_cast<uint32>(debugBtn)) > 1;
}

uint32 UAvengerDebugInputWidget::GetInputInfo() const
{
	uint32 btn = 0;

	for(int i = 0; ; i++)
	{
		if(DEBUG_INPUT_INFO[i].Bit == 0)
			break;

		if(PlayerController->IsInputKeyDown(DEBUG_INPUT_INFO[i].Key))
			btn |= DEBUG_INPUT_INFO[i].Bit;
	}

	return btn;
}