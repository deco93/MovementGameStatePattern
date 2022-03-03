// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENT_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
	UFUNCTION()
	void OnClick();

	void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* PlayButton;//button for resetting game on loss
};
