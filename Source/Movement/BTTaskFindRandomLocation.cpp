// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskFindRandomLocation.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <Runtime/NavigationSystem/Public/NavigationSystem.h>
#include "ZombieNPCAIController.h"
#include <BehaviorTree/Blackboard/BlackboardKeyType_Vector.h>
#include "BlackboardKeys.h"
#include "GameFramework/Character.h"


UBTTaskFindRandomLocation::UBTTaskFindRandomLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UBTTaskFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	auto const ZombieController = Cast<AZombieNPCAIController>(owner_comp.GetAIOwner());
	auto const ZombieNPC = ZombieController->GetPawn();

	FVector const ZombieLocation = ZombieNPC->GetActorLocation();
	FNavLocation RandomDestLocation;

	//get nav system and generate a rand location on navmesh
	UNavigationSystemV1* const Nav_Sys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (Nav_Sys->GetRandomPointInNavigableRadius(ZombieLocation, search_radius, RandomDestLocation, nullptr))
	{
		ZombieController->get_blackboard()->SetValueAsVector(bb_keys::target_location, RandomDestLocation.Location);
		ZombieController->GetCharacter()->SetActorRotation((RandomDestLocation.Location- ZombieLocation).Rotation());
	}

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
