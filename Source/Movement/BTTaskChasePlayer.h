// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BehaviorTreeTypes.h>
#include "BTTaskChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENT_API UBTTaskChasePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTaskChasePlayer(FObjectInitializer const& object_initializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory);
};
