// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "WaterItem.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENT_API UWaterItem : public UItem
{
	GENERATED_BODY()
public:
	UWaterItem();
	class AMovementCharacter* PlayerCharacter;
	class IPickupInterface* MappedPickupItem;
	void Use(class AMovementCharacter* Character) override;
	void SetMappedItem(class IPickupInterface* MappedItem) override;
};
