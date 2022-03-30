// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GunItem.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENT_API UGunItem : public UItem
{
	GENERATED_BODY()
protected:
	virtual void Use(class AMovementCharacter* Character) override;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClipMin = 0.0))
	float Damage;
};
