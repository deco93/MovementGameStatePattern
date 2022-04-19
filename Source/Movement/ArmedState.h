// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NinjaState.h"

/**
 * 
 */
class MOVEMENT_API ArmedState : public NinjaState
{
public:
	ArmedState();
	~ArmedState();
	virtual void HandleInput(AMovementCharacter* aMovementCharacter, InputTypeDirection inputType, float axisValue) override;
	virtual void Update(AMovementCharacter* aMovementCharacter) override;

	void EquipWeaponToHand(AMovementCharacter* aMovementCharacter);
	bool WeaponInHand = false;
	bool IsAiming = false;
	bool IsFiring = false;
};
