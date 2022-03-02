// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NinjaState.h"

/**
 * 
 */
class MOVEMENT_API SwimmingState: public NinjaState
{
public:
	SwimmingState() {}
	~SwimmingState() {}
	virtual void HandleInput(AMovementCharacter* aMovementCharacter, InputTypeDirection inputType, float axisValue) override;
	virtual void Update(AMovementCharacter* aMovementCharacter) override;

	float WaterZ = 0.0f;
	bool InWater = false;
};
