// Fill out your copyright notice in the Description page of Project Settings.


#include "BandageItem.h"
#include "MovementCharacter.h"
#include "PickupInterface.h"
#include "ArmedState.h"
#include "Components/BoxComponent.h"
#include "MovementGameMode.h"

UBandageItem::UBandageItem()
{
	
}

void UBandageItem::Use(class AMovementCharacter* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Bandage Used in c++"));
	/*FString BeforeMessage = FString::Printf(TEXT("Health before bandage use %f"), Character->Health);
	GEngine->AddOnScreenDebugMessage(0,2,FColor::Green, BeforeMessage );
	Character->Health += 10;
	Character->Health = FMath::Clamp(Character->Health,0.0f,100.0f);
	FString AfterMessage = FString::Printf(TEXT("Health after bandage use %f"), Character->Health);
	GEngine->AddOnScreenDebugMessage(1,2,FColor::Green,AfterMessage);*/


	//TODO equip to hand
	/*if (Character && Character->CurrentPickupItemInHand)//means current item in hand is same as the bandage being tried to Use
	{
		Character->CurrentPickupItemInHand->GetStaticMeshComp()->DetachFromParent();
		Character->CurrentPickupItemInHand = nullptr;
	}
	else if (Character && MappedPickupItem)
	{
		
		UStaticMeshComponent* SM = MappedPickupItem->GetStaticMeshComp();
		UBoxComponent* CollisionBoxComponent = MappedPickupItem->GetBoxComponent();
		if (Character->armedState && !Character->armedState->WeaponInHand)
		{
			SM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			CollisionBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			CollisionBoxComponent->SetGenerateOverlapEvents(false);
			SM->SetVisibility(true);
			SM->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandSocket"));
			Character->CurrentPickupItemInHand = MappedPickupItem;
		}
		else if (Character->armedState && Character->armedState->WeaponInHand)
		{
			Character->armedState->EquipWeaponToHand(Character);//this will unequip existing weapon in hand
			SM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			CollisionBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			CollisionBoxComponent->SetGenerateOverlapEvents(false);
			SM->SetVisibility(true);
			SM->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandSocket"));
			Character->CurrentPickupItemInHand = MappedPickupItem;
		}
	}*/
	if(Character && Character->CurrentPickupItemInHand)
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

void UBandageItem::SetMappedItem(IPickupInterface* MappedItem)
{
	MappedPickupItem = MappedItem;
}


