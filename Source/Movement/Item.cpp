// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

UItem::UItem() 
{
	Weight = 1.0f;
	ItemDisplayName = FText::FromString("Item");
	UseActionTextDetail = FText::FromString("Use");
}

void UItem::Use(AMovementCharacter* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Use of Item clicked, name: %s"), *ItemDisplayName.ToString());
}
