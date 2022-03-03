// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	PlayButton->OnClicked.AddDynamic(this, &UMainMenu::OnClick);
}

void UMainMenu::OnClick()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayButtonClicked"));
	UGameplayStatics::OpenLevel(GetWorld(), "ThirdPersonExampleMap");
}