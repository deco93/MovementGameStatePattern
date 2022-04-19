// Fill out your copyright notice in the Description page of Project Settings.


#include "GunItem.h"
#include "MovementCharacter.h"

void UGunItem::Use(class AMovementCharacter* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Gun Equipped, its damage: %f"), Damage);
	Character->EquipWeaponToHand();
}