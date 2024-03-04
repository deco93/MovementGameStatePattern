// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonFunctionLibrary.h"
#include "JsonObjectConverter.h"
#include "Misc/Paths.h"

void UCommonFunctionLibrary::PersistSpawnLocations(TArray<FSpawnLocation> LocationsArray)
{
	if (!LocationsArray.IsEmpty())
	{
		bool IsReadSuccess = false;
		FString ReadOutputMessage;
		FString ReadString = CommonUtils::ReadFile(FPaths::ProjectConfigDir() + "/DefaultSpawns.json", IsReadSuccess, ReadOutputMessage);

		TArray<TSharedPtr<FJsonValue>> ReadJsonArr;
		if (IsReadSuccess)
			FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(ReadString), ReadJsonArr);

		UE_LOG(LogTemp, Warning, TEXT("After existing spawn config read, number of existing spawns: %d"), ReadJsonArr.Num());
		for (int x = 0; x < LocationsArray.Num(); x++)
		{
			TSharedPtr<FJsonObject> TempLocationJsonObject = FJsonObjectConverter::UStructToJsonObject(LocationsArray[x]);
			TSharedPtr<FJsonValue> TempLocationJsonValue= MakeShareable(new FJsonValueObject(TempLocationJsonObject));
			ReadJsonArr.Add(TempLocationJsonValue);
		}
		UE_LOG(LogTemp, Warning, TEXT("After adding spawn config from editor utilities, number of spawns: %d"), ReadJsonArr.Num());
		FString SpawnsToWrite;
		if (!ReadJsonArr.IsEmpty())
		{
			if ( FJsonSerializer::Serialize(ReadJsonArr, TJsonWriterFactory<>::Create(&SpawnsToWrite, 1)) )
			{
				bool IsWriteSuccess = false;
				FString WriteOutputMessage;
				CommonUtils::WriteFile(FPaths::ProjectConfigDir() + "/DefaultSpawns.json", SpawnsToWrite, IsWriteSuccess, WriteOutputMessage);
				if (IsWriteSuccess)
				{
					UE_LOG(LogTemp, Warning, TEXT("FileWrite succeeded"));	
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("FileWrite did not succeed"));
				}
			}
		}
	}
}

TArray<FSpawnLocation> UCommonFunctionLibrary::GetSpawnLocations()
{
	TArray<FSpawnLocation> SpawnLocationsAr;
	bool IsReadSuccess = false;
	FString ReadOutputMessage;
	FString ReadString = CommonUtils::ReadFile(FPaths::ProjectConfigDir() + "/DefaultSpawns.json", IsReadSuccess, ReadOutputMessage);

	TArray<TSharedPtr<FJsonValue>> ReadJsonArr;
	if (IsReadSuccess)
		FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(ReadString), ReadJsonArr);

	UE_LOG(LogTemp, Warning, TEXT("After existing spawn config read, number of existing spawns: %d"), ReadJsonArr.Num());

	for (int x = 0; x < ReadJsonArr.Num(); x++)
	{
		const TSharedPtr<FJsonObject>* TempJsonObject;

		if (ReadJsonArr[x]->TryGetObject(TempJsonObject))
		{	
			
			FSpawnLocation TempSpawnLocation;
			if (FJsonObjectConverter::JsonObjectToUStruct(TempJsonObject->ToSharedRef(), &TempSpawnLocation))
			{
				UE_LOG(LogTemp, Warning, TEXT("jsonobject to struct convert successful:"));
				SpawnLocationsAr.Add(TempSpawnLocation);
			}
		}
		

		
	}
	UE_LOG(LogTemp, Warning, TEXT("spawn locations count returned for BP: %d"), SpawnLocationsAr.Num());
	return SpawnLocationsAr;
}
