// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
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

	UPROPERTY(EditAnywhere)
	class UAudioComponent* ZombieAudioComponent;

	float PATROL_SOUND_COOLDOWN = 7.0f;
	float PatrolSoundCurrentCoolOff = PATROL_SOUND_COOLDOWN;

	UPROPERTY(EditAnywhere)
	float Health = 500.0f;

	bool IsShot = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsChasing = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsDead = false;

	void TakeDamage(float Damage);

	FTimerHandle TIMER_HANDLE_Zombie;
	void OnZombieDeath();

	/*UPROPERTY(BlueprintReadWrite)
	bool SendAttack = false;*/
	

private:
	class UAIPerceptionStimuliSourceComponent* stimulus;
	void setup_stimulus();
	//void OnAttackAnimationComplete();
};
