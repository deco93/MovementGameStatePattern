// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieNPC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Perception/AIPerceptionStimuliSourceComponent.h>
#include "Kismet/GameplayStatics.h"
#include "MovementGameMode.h"
#include "MovementCharacter.h"
#include <Perception/AISense_Sight.h>


// Sets default values
AZombieNPC::AZombieNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//this->bUseControllerRotationYaw = true;
	ZombieAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ZombieAudioComponent"));
	RootComponent->SetupAttachment(ZombieAudioComponent);
	//setup_stimulus();
}

// Called when the game starts or when spawned
void AZombieNPC::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	GM = Cast<AMovementGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (ZombieAudioComponent && PatrolSound)
	{
		ZombieAudioComponent->SetSound(PatrolSound);
	}
}

// Called every frame
void AZombieNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const FVector MainCharLocation = GM->OurPlayerCharacter->GetActorLocation();
	const FVector ZombieLocation = GetActorLocation(); 

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

void AZombieNPC::setup_stimulus()
{
	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	stimulus->RegisterWithPerceptionSystem();
}

//void AZombieNPC::OnAttackAnimationComplete()
//{
//	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
//}

