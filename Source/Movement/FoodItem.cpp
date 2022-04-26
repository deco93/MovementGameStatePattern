// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodItem.h"
#include "MovementCharacter.h"
#include "PickupInterface.h"

UFoodItem::UFoodItem()
{

}

void UFoodItem::Use(AMovementCharacter* Character)
{

	if (Character && Character->CurrentPickupItemInHand)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Unequip called"));
		MappedPickupItem->UnEquip();
		Character->CurrentInventoryItemInHand = nullptr;
	}
	else if (Character && MappedPickupItem)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Equip called"));

		MappedPickupItem->Equip();
		Character->CurrentInventoryItemInHand = this;
	}
}

void UFoodItem::SetMappedItem(IPickupInterface* MappedItem)
{
	MappedPickupItem = MappedItem;
}
