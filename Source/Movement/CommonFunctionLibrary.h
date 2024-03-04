// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonUtils.h"
#include "CommonFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENT_API UCommonFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "CommonFunctionLibrary")
	static void PersistSpawnLocations(TArray<FSpawnLocation> LocationsArray);

	UFUNCTION(BlueprintCallable, Category = "CommonFunctionLibrary")
	static TArray<FSpawnLocation> GetSpawnLocations();
};
