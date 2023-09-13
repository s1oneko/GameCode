// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Widget/AvengerDebugObjListTabWidget.h"



void UAvengerDebugObjListTabWidget::OnClosedButtonPress()
{
	if(OnClosingTabWidget.IsBound())
		OnClosingTabWidget.Execute(this);
}

void UAvengerDebugObjListTabWidget::OnSwitchButtonPress()
{
	if(OnSwitchingObjWidget.IsBound())
		OnSwitchingObjWidget.Execute(ObjectRef.Get());
}
