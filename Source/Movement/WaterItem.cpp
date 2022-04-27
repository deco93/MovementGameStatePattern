// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterItem.h"
#include "MovementCharacter.h"
#include "PickupInterface.h"



UWaterItem::UWaterItem()
{

}

void UWaterItem::Use(AMovementCharacter* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Use called inside WaterItem"));
	if (Character && Character->CurrentPickupItemInHand)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Unequip called"));
		MappedPickupItem->UnEquip();
		//Character->CurrentInventoryItemInHand = nullptr;
	}
	else if (Character && MappedPickupItem)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Equip called"));

		MappedPickupItem->Equip();
		Character->CurrentInventoryItemInHand = this;
	}
}

void UWaterItem::SetMappedItem(IPickupInterface* MappedItem)
{
	MappedPickupItem = MappedItem;
}
