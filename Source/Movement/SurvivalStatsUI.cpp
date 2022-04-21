// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalStatsUI.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void USurvivalStatsUI::NativeConstruct()
{
	Super::NativeConstruct();
	//PlayButton->OnClicked.AddDynamic(this, &UMainMenu::OnClick);
	/*if (PickupText)
	{
		PickupText->SetVisibility(ESlateVisibility::Hidden);
	}*/

	if (Cuts)
		SetCutsVisibility(false);

	if (Food)
		SetFoodFill(1.0f);
	if (Water)
		SetWaterFill(1.0f);
	if (Blood)
		SetBloodFill(1.0f);
	if (Health)
		SetHealthFill(1.0f);
}

void USurvivalStatsUI::SetWaterFill(float FillAmount)
{
	if(Water)
		Water->SetPercent(FillAmount);
}

void USurvivalStatsUI::SetFoodFill(float FillAmount)
{
	if(Food)
		Food->SetPercent(FillAmount);
}

void USurvivalStatsUI::SetBloodFill(float FillAmount)
{
	if(Blood)
		Blood->SetPercent(FillAmount);
}

void USurvivalStatsUI::SetHealthFill(float FillAmount)
{
	if(Health)
		Health->SetPercent(FillAmount);
}

void USurvivalStatsUI::SetCutsVisibility(bool IsVisible)
{
	if (Cuts)
	{
		if (IsVisible)
			Cuts->SetVisibility(ESlateVisibility::Visible);
		else
			Cuts->SetVisibility(ESlateVisibility::Hidden);
	}
}
