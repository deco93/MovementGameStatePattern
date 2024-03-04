// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUtils.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSpawnLocation
{
	GENERATED_USTRUCT_BODY()
	FSpawnLocation() = default;
	FSpawnLocation(float LocationX, float LocationY, float LocationZ, FString Category, FString Tier, FString ItemClass):
	LocationX(LocationX), LocationY(LocationY), LocationZ(LocationZ), Category(Category), Tier(Tier), ItemClass(ItemClass){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CommonUtils")
	float LocationX = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CommonUtils")
	float LocationY = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CommonUtils")
	float LocationZ = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CommonUtils")
	FString Category;	//Military|Civil|Toxic|Industrial

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CommonUtils")
	FString Tier;	//Tier1|Tier2|Tier3|Tier4|Rare

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CommonUtils")
	FString ItemClass;	//Class of Item to be spawned
};

class MOVEMENT_API CommonUtils
{
public:
	CommonUtils();
	~CommonUtils();
	static FString ReadFile(FString FilePath, bool& IsSuccess, FString& OutputMessage);
	static void WriteFile(FString FilePath, FString ContentToWrite, bool& IsSuccess, FString& OutputMessage);
};
