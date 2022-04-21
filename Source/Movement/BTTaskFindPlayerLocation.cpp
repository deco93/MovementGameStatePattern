// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskFindPlayerLocation.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <Runtime/NavigationSystem/Public/NavigationSystem.h>
#include "ZombieNPCAIController.h"
#include <BehaviorTree/Blackboard/BlackboardKeyType.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Vector.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <GameFramework/Character.h>
#include "MovementGameMode.h"
#include "MovementCharacter.h"
#include "BlackboardKeys.h"

UBTTaskFindPlayerLocation::UBTTaskFindPlayerLocation(FObjectInitializer const& object_initializer)
{
    NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTTaskFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
    /*ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
    auto const PC = Cast<AZombieNPCAIController>(owner_comp.GetAIOwner());

    FVector const PlayerLocation = Player->GetActorLocation();
    AMovementGameMode* const GM = Cast<AMovementGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    AMovementCharacter* const OurPlayer = GM->OurPlayerCharacter;
    const float DistanceToMainPlayer = FVector::Dist(PlayerLocation, OurPlayer->GetActorLocation());
    UE_LOG(LogTemp, Warning, TEXT("___Distance to MainPlayer %f"), DistanceToMainPlayer);
    if (DistanceToMainPlayer <= search_radius)
    {
        PC->get_blackboard()->SetValueAsVector(bb_keys::target_location, OurPlayer->GetActorLocation());
        PC->get_blackboard()->SetValueAsBool(bb_keys::can_move_to_player, true);
    }
    else
    {
        PC->get_blackboard()->SetValueAsBool(bb_keys::can_move_to_player, false);
    }*/
    /*if (search_random)
    {
        FNavLocation DestLocation;

        UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
        if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, search_radius, DestLocation, nullptr))
        {
            PC->get_blackboard()->SetValueAsVector(bb_keys::target_location, DestLocation.Location);
        }
    }
    else
    {
        PC->get_blackboard()->SetValueAsVector(bb_keys::target_location, PlayerLocation);
    }*/

    FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}
