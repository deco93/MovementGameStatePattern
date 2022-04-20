// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairUI.h"
#include "Components/Image.h"

void UCrosshairUI::NativeConstruct()
{
	Super::NativeConstruct();
	//PlayButton->OnClicked.AddDynamic(this, &UMainMenu::OnClick);
	/*if (PickupText)
	{
		PickupText->SetVisibility(ESlateVisibility::Hidden);
	}*/
}

void UCrosshairUI::ShowCrosshair(bool IsShow)
{
	if (Crosshair)
	{
		//if (!Crosshair->IsVisible())
		if (IsShow)
		{
			Crosshair->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Crosshair->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
