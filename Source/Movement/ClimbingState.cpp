// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbingState.h"
#include "Components/CapsuleComponent.h"
#include "MovementGameMode.h"

void ClimbingState::HandleInput(AMovementCharacter* aMovementCharacter, InputTypeDirection inputType, float axisValue)
{
	UE_LOG(LogTemp, Warning, TEXT("inside HandleInput of climbing state"));
	if (inputType == InputTypeDirection::FORWARD_ITD)
	{
		aMovementCharacter->SetTimer();
	}
	else if (inputType == InputTypeDirection::RIGHT_ITD)
	{
		FVector StartLocation;
		FVector Direction;
		if (axisValue > 0.0f)
		{
			StartLocation = aMovementCharacter->GetActorLocation() + aMovementCharacter->GetActorRightVector() * 100.0f;
			LedgeHorizontalSpeed = 100.0f;
		}
		else if (axisValue < 0.0f)
		{
			StartLocation = aMovementCharacter->GetActorLocation() - (aMovementCharacter->GetActorRightVector() * 100.0f);
			LedgeHorizontalSpeed = -100.0f;
		}

		StartLocation.Z = StartLocation.Z + (aMovementCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - 15.0f);//raise the startlocation to about ledge hand location Z
		FVector EndLocation = StartLocation + aMovementCharacter->GetActorForwardVector() * aMovementCharacter->TraceDistance;
		FHitResult OHit = aMovementCharacter->GM->DrawLineTrace(StartLocation, EndLocation);
		if (OHit.bBlockingHit && !OHit.Actor->ActorHasTag("Ledge"))
			axisValue = 0.0f;
		else if (!OHit.bBlockingHit)
			axisValue = 0.0f;

		Direction = aMovementCharacter->GetActorRightVector();
		aMovementCharacter->AddMovementInput(Direction, axisValue);
	}
}

void ClimbingState::Update(AMovementCharacter* aMovementCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("inside Update of climbing state"));
}
