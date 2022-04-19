// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupUI.h"
#include "Components/TextBlock.h"

void UPickupUI::NativeConstruct()
{
	Super::NativeConstruct();
	//PlayButton->OnClicked.AddDynamic(this, &UMainMenu::OnClick);
	if (PickupText)
	{
		PickupText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPickupUI::ShowPickupText()
{
	UE_LOG(LogTemp, Warning, TEXT("Inside ShowPickupText"));
	if (PickupText)
	{
		UE_LOG(LogTemp, Warning, TEXT("PickupText exists"));
		if (!PickupText->IsVisible())
		{
			PickupText->SetVisibility(ESlateVisibility::Visible);
		}
		else if (PickupText->IsVisible())
		{
			PickupText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
