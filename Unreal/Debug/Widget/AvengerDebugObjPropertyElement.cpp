// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Widget/AvengerDebugObjPropertyElement.h"

#include "Components/TextBlock.h"

void UAvengerDebugObjPropertyElement::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	auto const obj = Cast<UAvengerDebugObjPropertyElementData>(ListItemObject);

	StrValueText->SetText(FText::FromString(obj->PropertyValue));
}