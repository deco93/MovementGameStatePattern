// Fill out your copyright notice in the Description page of Project Settings.


#include "BandageItem.h"
#include "MovementCharacter.h"

void UBandageItem::Use(class AMovementCharacter* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Bandage Used in c++"));
	FString BeforeMessage = FString::Printf(TEXT("Health before bandage use %f"), Character->Health);
	GEngine->AddOnScreenDebugMessage(0,2,FColor::Green, BeforeMessage );
	Character->Health += 10;
	Character->Health = FMath::Clamp(Character->Health,0.0f,100.0f);
	FString AfterMessage = FString::Printf(TEXT("Health after bandage use %f"), Character->Health);
	GEngine->AddOnScreenDebugMessage(1,2,FColor::Green,AfterMessage);
}