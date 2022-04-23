// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskChasePlayer.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "ZombieNPCAIController.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include "BlackboardKeys.h"

UBTTaskChasePlayer::UBTTaskChasePlayer(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTTaskChasePlayer::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	AZombieNPCAIController* const ZombieNPCAIController = Cast<AZombieNPCAIController>(owner_comp.GetAIOwner());
	FVector const player_location = ZombieNPCAIController->get_blackboard()->GetValueAsVector(bb_keys::target_location);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(ZombieNPCAIController, player_location);
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

