// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonUtils.h"
#include "Misc/FileHelper.h"

CommonUtils::CommonUtils()
{
}

CommonUtils::~CommonUtils()
{
}

FString CommonUtils::ReadFile(FString FilePath, bool& IsSuccess, FString& OutputMessage)
{
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
	{
		IsSuccess = false;
		OutputMessage = FString::Printf(TEXT("File read failed, file doesnt exist at: %s"), *FilePath);
		return "";
	}

	FString FileContents = "";

	if (!FFileHelper::LoadFileToString(FileContents, *FilePath))
	{
		IsSuccess = false;
		OutputMessage = FString::Printf(TEXT("File read failed, is this a text file at this location: %s ?"), *FilePath);
		return "";
	}

	IsSuccess = true;
	OutputMessage = FString::Printf(TEXT("File read succeeded from file location: %s ?"), *FilePath);
	return FileContents;
}

void CommonUtils::WriteFile(FString FilePath, FString ContentToWrite, bool& IsSuccess, FString& OutputMessage)
{
	if (!FFileHelper::SaveStringToFile(ContentToWrite, *FilePath))
	{
		IsSuccess = false;
		OutputMessage = FString::Printf(TEXT("Write to file failed, at file location: %s"), *FilePath);
		return;
	}
	IsSuccess = true;
	OutputMessage = FString::Printf(TEXT("Write to file succeeded, at file location: %s"), *FilePath);
}
