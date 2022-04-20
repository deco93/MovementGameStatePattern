// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairUI.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENT_API UCrosshairUI : public UUserWidget
{
	GENERATED_BODY()
	void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UImage* Crosshair;

	UFUNCTION(BlueprintCallable)
	void ShowCrosshair(bool IsShow);
};
