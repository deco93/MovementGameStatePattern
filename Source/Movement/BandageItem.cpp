// Fill out your copyright notice in the Description page of Project Settings.


#include "BandageItem.h"
#include "MovementCharacter.h"
#include "PickupInterface.h"
//#include "ArmedState.h"
#include "Components/BoxComponent.h"
#include "MovementGameMode.h"

UBandageItem::UBandageItem()
{
	
}

void UBandageItem::Use(class AMovementCharacter* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Bandage Used in c++"));
	
	if(Character && Character->CurrentPickupItemInHand)
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

void UBandageItem::SetMappedItem(IPickupInterface* MappedItem)
{
	MappedPickupItem = MappedItem;
}


