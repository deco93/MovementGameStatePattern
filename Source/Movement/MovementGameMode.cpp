// Copyright Epic Games, Inc. All Rights Reserved.

#include "MovementGameMode.h"
#include "MovementCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

AMovementGameMode::AMovementGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

FHitResult AMovementGameMode::DrawLineTrace(FVector Start, FVector End, FColor Color, bool PersistentLine, float LifeTime)
{
	FHitResult OHit;
	
		FCollisionQueryParams TraceParams;
		UWorld* uWorld = GetWorld();
		bool HitSuccess = uWorld->LineTraceSingleByChannel(OHit, Start, End, ECC_Visibility, TraceParams);
		if (VisualizeDebug)
		{
			DrawDebugLine(uWorld, Start, End, Color, PersistentLine, LifeTime);
			if (HitSuccess)
			{
				DrawDebugBox(uWorld, OHit.ImpactPoint, FVector(5, 5, 5), FColor::Green, PersistentLine, LifeTime);
			}
		}
	return OHit;
}

FHitResult AMovementGameMode::DrawLineTraceForFiring(FVector Start, FVector End, FColor Color, bool PersistentLine, float LifeTime)
{
	FHitResult OHit;

	FCollisionQueryParams TraceParams;
	UWorld* uWorld = GetWorld();
	bool HitSuccess = uWorld->LineTraceSingleByChannel(OHit, Start, End, ECC_Pawn, TraceParams);
	if (VisualizeDebug)
	{
		DrawDebugLine(uWorld, Start, End, Color, PersistentLine, LifeTime);
		if (HitSuccess)
		{
			DrawDebugBox(uWorld, OHit.ImpactPoint, FVector(5, 5, 5), FColor::Green, PersistentLine, LifeTime);
		}
	}
	return OHit;
}

void AMovementGameMode::SetPlayerCharacter(AMovementCharacter* i_OurPlayerCharacter)
{
	OurPlayerCharacter = i_OurPlayerCharacter;
}
