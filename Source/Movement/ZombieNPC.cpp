// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieNPC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Perception/AIPerceptionStimuliSourceComponent.h>
#include "Kismet/GameplayStatics.h"
#include "MovementGameMode.h"
#include "MovementCharacter.h"
#include "PickupWater.h"
#include <Perception/AISense_Sight.h>


// Sets default values
AZombieNPC::AZombieNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//this->bUseControllerRotationYaw = true;
	ZombieAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ZombieAudioComponent"));

	/*static ConstructorHelpers::FObjectFinder<APickupWater> PickupWaterBP(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/Spawnables/BP_WaterBottlePickup.BP_WaterBottlePickup'"));
	if (PickupWaterBP.Succeeded())
	{
		PickupWater = PickupWaterBP.Object;
	}*/
	//setup_stimulus();
	if (ZombieAudioComponent)
	{
		ZombieAudioComponent->bOverrideAttenuation = 1;
		ZombieAudioComponent->bAllowSpatialization = 1;
		ZombieAudioComponent->AttenuationOverrides.AttenuationShape = EAttenuationShape::Type::Sphere;
		ZombieAudioComponent->AttenuationOverrides.DistanceAlgorithm = EAttenuationDistanceModel::Linear;
		ZombieAudioComponent->AttenuationOverrides.ConeSphereRadius = 400.0f;
		ZombieAudioComponent->AttenuationOverrides.FalloffDistance = 800.0f;
		/*PatrolSound->bOverrideAttenuation = 1;
		PatrolSound->AttenuationOverrides.AttenuationShape = EAttenuationShape::Type::Sphere;
		PatrolSound->AttenuationOverrides.DistanceAlgorithm = EAttenuationDistanceModel::Linear;
		PatrolSound->AttenuationOverrides.ConeSphereRadius = 400.0f;
		PatrolSound->AttenuationOverrides.FalloffDistance = 800.0f;*/
	}
	ZombieAudioComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AZombieNPC::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	GM = Cast<AMovementGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	/*if (ZombieAudioComponent && PatrolSound)
	{	
		ZombieAudioComponent->SetSound(PatrolSound);
		ZombieAudioComponent->sound
	}*/
}

// Called every frame
void AZombieNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const FVector MainCharLocation = GM->OurPlayerCharacter->GetActorLocation();
	const FVector ZombieLocation = GetActorLocation(); 
	if (Health <= 0.0f && IsDead == false)
	{
		IsDead = true;
		GetCharacterMovement()->StopMovementImmediately();
		GetWorld()->GetTimerManager().SetTimer(TIMER_HANDLE_Zombie, this, &AZombieNPC::OnZombieDeath, 3.6f, false);
	}
	/*if (FVector::Dist(ZombieLocation, MainCharLocation) < 150.0f)
	{	
		if (AttackAnimation)
		{
			GetMesh()->PlayAnimation(AttackAnimation, false);
			GetWorld()->GetTimerManager().SetTimer(GM->TIMER_HANDLE, this, &AZombieNPC::OnAttackAnimationComplete, 3.0f, false);
		}
	}*/
	
}

// Called to bind functionality to input
void AZombieNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AZombieNPC::TakeDamage(float Damage)
{
	IsShot = true;
	Health -= Damage;
	Health =FMath::Clamp(Health, 0.0f, 500.0f);
	//playing animation and sound effect done in BehaviorTree handled by ZombieNPCAIController
}

void AZombieNPC::setup_stimulus()
{
	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	stimulus->RegisterWithPerceptionSystem();
}

void AZombieNPC::OnZombieDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("OnZombieDeath called"));
	//TODO spawn a food/bandage/fishtrap/bottle
	/*FVector DirectionTowardsPlayer = (GM->OurPlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FVector LocationApproxBetweenZombieAndPlayer = GetActorLocation() + DirectionTowardsPlayer * 25.0f;
	FRotator PlayerRotation(0, GM->OurPlayerCharacter->GetActorRotation().Yaw,0);
	GetWorld()->SpawnActor<APickupWater>(WaterBottleToSpawn, LocationApproxBetweenZombieAndPlayer, PlayerRotation);*/
	Destroy();
}

//void AZombieNPC::OnAttackAnimationComplete()
//{
//	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
//}

