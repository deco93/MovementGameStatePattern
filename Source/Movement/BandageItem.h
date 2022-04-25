// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "PickupInterface.h"
#include "BandageItem.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENT_API UBandageItem : public UItem
{
	GENERATED_BODY()
public :
	class AMovementCharacter* PlayerCharacter;
	UBandageItem();
protected:
	virtual void Use(class AMovementCharacter* Character) override;

};
