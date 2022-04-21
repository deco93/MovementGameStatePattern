// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieNPCAIController.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <UObject/ConstructorHelpers.h>
#include <Runtime/NavigationSystem/Public/NavigationSystem.h>
#include "ZombieNPC.h"
#include "MovementCharacter.h"
#include "MovementGameMode.h"
#include <Perception/AISenseConfig_Sight.h>
#include <Perception/AIPerceptionStimuliSourceComponent.h>
#include <Perception/AIPerceptionComponent.h>
#include "BlackboardKeys.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/Engine/Classes/Engine/World.h>
#include <GameFramework/Character.h>

AZombieNPCAIController::AZombieNPCAIController(FObjectInitializer const& object_initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/ThirdPersonCPP/Blueprints/AI/BT_ZombieNPC.BT_ZombieNPC'"));
	if (obj.Succeeded())
	{
		btree = obj.Object;
	}
	behavior_tree_component = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));
	//setup_perception_system();
}

void AZombieNPCAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(btree);
	behavior_tree_component->StartTree(*btree);
}

void AZombieNPCAIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	if(blackboard)
	{ 
		blackboard->InitializeBlackboard(*btree->BlackboardAsset);
	}
}

UBlackboardComponent* AZombieNPCAIController::get_blackboard() const
{
	return blackboard;
}

void AZombieNPCAIController::Tick(float DeltaSeconds)
{
	AMovementGameMode* const GM = Cast<AMovementGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	AMovementCharacter* const OurPlayer = GM->OurPlayerCharacter;
	const float DistanceToMainPlayer = FVector::Dist(this->GetCharacter()->GetActorLocation(), OurPlayer->GetActorLocation());
	AZombieNPC* const ZombieNPC = Cast<AZombieNPC>(this->GetCharacter());
	UE_LOG(LogTemp, Warning, TEXT("___Distance to MainPlayer %f"), DistanceToMainPlayer);
	UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation RandomDestLocation;
	//else if (DistanceToMainPlayer <= search_radius_to_move_to_player )
	if (DistanceToMainPlayer < attack_distance)
	{
		get_blackboard()->SetValueAsBool(bb_keys::can_attack, true);
		get_blackboard()->SetValueAsBool(bb_keys::can_move_to_player, false);
		GetWorld()->GetTimerManager().SetTimer(GM->TIMER_HANDLE, this, &AZombieNPCAIController::OnAttackAnimationPlay, 1.0f, false);
	}
	else if (DistanceToMainPlayer <= search_radius_to_move_to_player && NavSys->GetRandomPointInNavigableRadius(OurPlayer->GetActorLocation(), 5.0f, RandomDestLocation, nullptr))
	{
		if (ZombieNPC->PatrolSoundCurrentCoolOff == ZombieNPC->PATROL_SOUND_COOLDOWN) 
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ZombieNPC->PatrolSound);
			ZombieNPC->PatrolSoundCurrentCoolOff -= DeltaSeconds;
		}
		else if (ZombieNPC->PatrolSoundCurrentCoolOff > 0.0f && ZombieNPC->PatrolSoundCurrentCoolOff < ZombieNPC->PATROL_SOUND_COOLDOWN)
		{
			ZombieNPC->PatrolSoundCurrentCoolOff -= DeltaSeconds;
		}
		else 
		{
			//RESET to max cooldown so it can be played again
			ZombieNPC->PatrolSoundCurrentCoolOff = ZombieNPC->PATROL_SOUND_COOLDOWN;
		}
		get_blackboard()->SetValueAsVector(bb_keys::target_location, OurPlayer->GetActorLocation()+(this->GetCharacter()->GetActorLocation()- OurPlayer->GetActorLocation()).Normalize()*110.0f);
		get_blackboard()->SetValueAsBool(bb_keys::can_attack, false);
		get_blackboard()->SetValueAsBool(bb_keys::can_move_to_player, true);
	}
	else
	{
		get_blackboard()->SetValueAsBool(bb_keys::can_attack, false);
		get_blackboard()->SetValueAsBool(bb_keys::can_move_to_player, false);
	}
}

void AZombieNPCAIController::on_updated(TArray<AActor*> const& updated_actors)
{

}

void AZombieNPCAIController::on_target_detected(AActor* actor, FAIStimulus const stimulus)
{
	if (auto const ch = Cast<AMovementCharacter>(actor))
	{
		get_blackboard()->SetValueAsBool(bb_keys::can_see_player, stimulus.WasSuccessfullySensed());
	}
}

void AZombieNPCAIController::setup_perception_system()
{
	sight_config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	sight_config->SightRadius = 500.0f;
	sight_config->LoseSightRadius = sight_config->SightRadius+ 50.0f;
	sight_config->PeripheralVisionAngleDegrees = 90.0f;
	sight_config->SetMaxAge(5.0f);
	sight_config->AutoSuccessRangeFromLastSeenLocation = 500.0f;
	sight_config->DetectionByAffiliation.bDetectEnemies = true;
	sight_config->DetectionByAffiliation.bDetectFriendlies = true;
	sight_config->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*sight_config->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AZombieNPCAIController::on_target_detected);
	GetPerceptionComponent()->ConfigureSense(*sight_config);

}

void AZombieNPCAIController::OnAttackAnimationPlay()
{
	UGameplayStatics::PlaySound2D(GetWorld(), Cast<AZombieNPC>(this->GetCharacter())->HitSound);
}
