// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MOVEMENT_API IPickupInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//virtual ~IPickupInterface() {};
	UFUNCTION()
	virtual void Consume() = 0;
	UFUNCTION()
	virtual void Pickup() = 0;
	UFUNCTION()
	virtual void Equip() = 0;
	UFUNCTION()
	virtual void UnEquip() = 0;
	UFUNCTION()
	virtual bool InHand() = 0;
};
