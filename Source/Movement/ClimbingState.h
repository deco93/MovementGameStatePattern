// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NinjaState.h"

/**
 * 
 */
class MOVEMENT_API ClimbingState: public NinjaState
{
public:
	ClimbingState() {}
	~ClimbingState() {}
	virtual void HandleInput(AMovementCharacter* aMovementCharacter, InputTypeDirection inputType, float axisValue) override;
	virtual void Update(AMovementCharacter* aMovementCharacter) override;

	bool ClimbUp = false;
	FVector LastLedgeClimbPosition;
	float LedgeHorizontalSpeed =0.0f;
};
