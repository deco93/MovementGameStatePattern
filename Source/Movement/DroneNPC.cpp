// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneNPC.h"
#include "Components/AudioComponent.h"

// Sets default values
ADroneNPC::ADroneNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DroneAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DroneAudioComponent"));

	if (DroneAudioComponent)
	{
		DroneAudioComponent->bOverrideAttenuation = 1;
		DroneAudioComponent->bAllowSpatialization = 1;
		DroneAudioComponent->AttenuationOverrides.AttenuationShape = EAttenuationShape::Type::Sphere;
		DroneAudioComponent->AttenuationOverrides.DistanceAlgorithm = EAttenuationDistanceModel::Linear;
		DroneAudioComponent->AttenuationOverrides.ConeSphereRadius = 400.0f;
		DroneAudioComponent->AttenuationOverrides.FalloffDistance = 800.0f;
	}
	DroneAudioComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADroneNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADroneNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADroneNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

