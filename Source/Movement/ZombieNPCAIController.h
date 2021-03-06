// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <Perception/AIPerceptionTypes.h>
#include "ZombieNPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class MOVEMENT_API AZombieNPCAIController : public AAIController
{
	GENERATED_BODY()
public:
	AZombieNPCAIController(FObjectInitializer const& object_initializer= FObjectInitializer::Get());
	void BeginPlay() override;
	void OnPossess(APawn* const pawn) override;
	class UBlackboardComponent* get_blackboard() const;
	virtual void Tick(float DeltaSeconds) override;

	float search_radius_to_move_to_player = 2000.0f;
	float attack_distance = 180.0f;
private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* behavior_tree_component;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* btree;

	UPROPERTY()
	class UBlackboardComponent* myblackboard;
	UPROPERTY()
	class UAISenseConfig_Sight* sight_config;

	UFUNCTION()
	void on_updated(TArray<AActor*> const& updated_actors);

	UFUNCTION()
	void on_target_detected(AActor* actor, FAIStimulus const stimulus);

	void setup_perception_system();
	void AfterBeingShot();

	//void OnAttacking();
	UPROPERTY()
	class AMovementGameMode* GM;
	FTimerHandle TIMER_HANDLE_AI;
	//void OnAttackAnimationPlay();
};
