// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MovementGameMode.generated.h"

UCLASS(minimalapi)
class AMovementGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMovementGameMode();
	FHitResult DrawLineTrace(FVector Start, FVector End, FColor Color = FColor::Red, bool PersistentLine = false, float LifeTime = 0.5f);
	bool VisualizeDebug = false;
	float DefaultTraceDistance = 2000.f;
	FTimerHandle TIMER_HANDLE;

	
};



