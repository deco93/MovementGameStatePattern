// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementCharacter.h"
/**
 * 
 */
enum InputTypeDirection {
	UP_ITD,
	RIGHT_ITD,
	FORWARD_ITD
};
class MOVEMENT_API NinjaState
{
public:
	//NinjaState();
	virtual ~NinjaState() {}
	virtual void HandleInput(AMovementCharacter* aMovementCharacter, InputTypeDirection inputType, float axisValue) {}
	virtual void Update(AMovementCharacter* aMovementCharacter) {}
};
