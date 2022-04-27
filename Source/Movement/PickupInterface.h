// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MOVEMENT_API IPickupInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//virtual ~IPickupInterface() {};

	UFUNCTION()
	virtual void Consume() = 0;
	UFUNCTION()
	virtual void Pickup() = 0;
	UFUNCTION()
	virtual void Equip() = 0;
	UFUNCTION()
	virtual void UnEquip() = 0;
	UFUNCTION()
	virtual bool InHand() = 0;
	UFUNCTION()
	virtual void DetachFromCharacterSocket() = 0;
	UFUNCTION()
	virtual class UStaticMeshComponent* GetStaticMeshComp() = 0;
	UFUNCTION()
	virtual class UBoxComponent* GetBoxComponent() = 0;
	UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) = 0;
	UFUNCTION()
	virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) = 0;
};
