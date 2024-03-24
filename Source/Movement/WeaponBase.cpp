// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "MovementCharacter.h"
#include "Projectile.h"



void AWeaponBase::Server_SetClearPotentialWeapon_Implementation(AMovementCharacter* Character, int SetOrClearFlag)
{
	if (Character)
	{
		if (SetOrClearFlag == 0)
			Character->PotentialWeapon = this;
		else if (SetOrClearFlag == 1)
			Character->PotentialWeapon = nullptr;
	}
}

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true);
	RootComponent = WeaponSkeletalMeshComp;
	WeaponSkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponSkeletalMeshComp");
	WeaponBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>("WeaponBoxCollisionComponent");
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	WeaponBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnOverlapBegin);
	WeaponBoxCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AWeaponBase::OnOverlapEnd);
}

void AWeaponBase::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("inside WeaponBase Fire"));
}

void AWeaponBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (!WeaponPickedUp)
	{
		
		if (OtherActor)
		{
			AMovementCharacter* Char = Cast<AMovementCharacter>(OtherActor);
			if (Char && WeaponInventoryItem)
			{	
				//Char->PotentialWeapon = this;
				Server_SetClearPotentialWeapon(Char, 0);
				Char->TriggerPickup(GetActorLocation());
			}
		}
	}
}

void AWeaponBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("You no longer overlap with weapon"));
	if (!WeaponPickedUp && OtherActor)
	{
		AMovementCharacter* Char = Cast<AMovementCharacter>(OtherActor);
		
		if (Char)
		{	
			//Char->PotentialWeapon = nullptr;
			//Char->Weapon = nullptr;
			Server_SetClearPotentialWeapon(Char, 1);
			Char->TriggerPickup(GetActorLocation());
		}
	}
}

UGunItem* AWeaponBase::GetWeaponInventoryItem()
{
	UGunItem* Item = nullptr;
	if (WeaponInventoryItem)
		Item = WeaponInventoryItem;
	return Item;
}

//void AWeaponBase::Server_SpawnProjectile_Implementation(FVector Location, FRotator Rotation)
//{
//	UWorld* CurrentWorld = GetWorld();
//	UE_LOG(LogTemp, Error, TEXT("inside %s"), ANSI_TO_TCHAR(__FUNCTION__));
//	if (CurrentWorld)
//	{
//		UE_LOG(LogTemp, Error, TEXT("CurrentWorld not null"));
//		if (ProjectileToSpawn)
//		{
//			UE_LOG(LogTemp, Error, TEXT("SpawnActor called for projectile"));
//			CurrentWorld->SpawnActor<AProjectile>(ProjectileToSpawn, Location, Rotation);
//		}
//	}
//}



