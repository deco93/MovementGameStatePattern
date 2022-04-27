// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "BandageItem.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENT_API UBandageItem : public UItem
{
	GENERATED_BODY()
public :
	UBandageItem();

	UPROPERTY()
	class AMovementCharacter* PlayerCharacter;

	//UPROPERTY()
	class IPickupInterface* MappedPickupItem;

	//UFUNCTION()
	void Use(class AMovementCharacter* Character) override;
	//UFUNCTION()
	void SetMappedItem(class IPickupInterface* MappedItem) override;

};
