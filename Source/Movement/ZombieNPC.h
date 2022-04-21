// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "ZombieNPC.generated.h"

UCLASS()
class MOVEMENT_API AZombieNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	class AMovementCharacter* OurPlayerCharacter;
	class AMovementGameMode* GM;
	UPROPERTY(EditDefaultsOnly)
	class UAnimationAsset* AttackAnimation;

	class AController* NPCAiController;
	class APawn* NPCAiPawn;

	UPROPERTY(EditAnywhere)
	USoundCue* PatrolSound;

	UPROPERTY(EditAnywhere)
	USoundCue* HitSound;

	float PATROL_SOUND_COOLDOWN = 7.0f;
	float PatrolSoundCurrentCoolOff = PATROL_SOUND_COOLDOWN;


private:
	class UAIPerceptionStimuliSourceComponent* stimulus;
	void setup_stimulus();

	//void OnAttackAnimationComplete();
};