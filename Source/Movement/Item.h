// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS(Abstract,BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced )
class MOVEMENT_API UItem : public UObject
{
	GENERATED_BODY()
public:
	UItem();

	virtual class UWorld* GetWorld() const { return World; };

	UPROPERTY(Transient)
	class UWorld* World;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText UseActionTextDetail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	class UStaticMesh* PickupMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	class UTexture2D* Thumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText ItemDisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta=(Multiline = true))
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta=(ClipMin = 0.0))
	float Weight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	class UInventoryComponent* OwningInventory;

	virtual void Use(class AMovementCharacter* Character);

	UFUNCTION(BlueprintImplementableEvent)//this one is for implementation in BP
	void OnUse(class AMovementCharacter* Character);

	virtual void SetMappedItem(class IPickupInterface* MappedItem);
};
