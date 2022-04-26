// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "FoodItem.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENT_API UFoodItem : public UItem
{
	GENERATED_BODY()
public:
	UFoodItem();
	class AMovementCharacter* PlayerCharacter;
	class IPickupInterface* MappedPickupItem;
	void Use(class AMovementCharacter* Character) override;
	void SetMappedItem(class IPickupInterface* MappedItem) override;
};
