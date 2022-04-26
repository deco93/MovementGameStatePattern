// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SurvivalStatsUI.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENT_API USurvivalStatsUI : public UUserWidget
{
	GENERATED_BODY()
	
	void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CutsCount;

	UPROPERTY(meta = (BindWidget))
	class UImage* Cuts;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Food;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Water;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Blood;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Health;

	UFUNCTION(BlueprintCallable)
	void SetWaterFill(float FillAmount);

	UFUNCTION(BlueprintCallable)
	void SetFoodFill(float FillAmount);

	UFUNCTION(BlueprintCallable)
	void SetBloodFill(float FillAmount);

	UFUNCTION(BlueprintCallable)
	void SetHealthFill(float FillAmount);

	UFUNCTION(BlueprintCallable)
	void SetCutsVisibility(bool IsVisible);

	UFUNCTION(BlueprintCallable)
	void SetCutsCount(int i_Cuts);

	UFUNCTION(BlueprintCallable)
	void SetCutsCountVisibility(bool IsVisible);
};
