// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupUI.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENT_API UPickupUI : public UUserWidget
{
	GENERATED_BODY()
	

	void NativeConstruct() override;

	UPROPERTY(meta= (BindWidget))
	class UTextBlock* PickupText;

	UFUNCTION(BlueprintCallable)
	void ShowPickupText();

};
