// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmedState.h"
#include "MovementCharacter.h"
#include "PickupInterface.h"
#include "WeaponBase.h"

ArmedState::ArmedState()
{
}

ArmedState::~ArmedState()
{
}

void ArmedState::HandleInput(AMovementCharacter* aMovementCharacter, InputTypeDirection inputType, float axisValue)
{

}

void ArmedState::Update(AMovementCharacter* aMovementCharacter)
{

}

void ArmedState::EquipWeaponToHand(AMovementCharacter* aMovementCharacter)
{
	if (aMovementCharacter->Weapon)
	{
		if (!WeaponInHand)
		{
			//if (aMovementCharacter->CurrentPickupItemInHand && aMovementCharacter->CurrentPickupItemInHand->GetStaticMeshComp())
			if (aMovementCharacter->CurrentPickupItemInHand)
			{
				aMovementCharacter->CurrentPickupItemInHand->DetachFromCharacterSocket();
				//aMovementCharacter->CurrentPickupItemInHand = nullptr;
			}
			aMovementCharacter->Weapon->AttachToComponent(aMovementCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandSocket"));
			WeaponInHand = true;
		}
		else
		{
			aMovementCharacter->Weapon->AttachToComponent(aMovementCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightShoulderSocket"));
			WeaponInHand = false;
			if (IsAiming)
				IsAiming = false;

			if (IsFiring)
				IsFiring = false;
		}
	}
}
