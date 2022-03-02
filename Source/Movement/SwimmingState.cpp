// Fill out your copyright notice in the Description page of Project Settings.


#include "SwimmingState.h"


void SwimmingState::HandleInput(AMovementCharacter* aMovementCharacter, InputTypeDirection inputType, float axisValue)
{
	UE_LOG(LogTemp, Warning, TEXT("inside HandleInput of swimming state"));
	if (inputType == InputTypeDirection::UP_ITD)
	{
		const FRotator Rotation = aMovementCharacter->Controller->GetControlRotation();
		const FRotator RollRotation(Rotation.Roll, 0, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(RollRotation).GetUnitAxis(EAxis::Z);
		// add movement in that direction
		aMovementCharacter->AddMovementInput(Direction, axisValue);
	}
	UE_LOG(LogTemp, Warning, TEXT("inside HandleInput of swimming state after addmovement input"));
}

void SwimmingState::Update(AMovementCharacter* aMovementCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("inside Update of swimming state"));
}
