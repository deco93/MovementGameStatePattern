// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "MovementCharacter.h"
#include "Projectile.h"



// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	RootComponent = WeaponSkeletalMeshComp;
	WeaponSkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponSkeletalMeshComp");
	//WeaponSkeletalMeshComp->SetupAttachment(RootComponent);

	WeaponBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>("WeaponBoxCollisionComponent");
	//WeaponBoxCollisionComponent->SetupAttachment(WeaponSkeletalMeshComp);
	//WeaponBoxCollisionComponent->SetRelativeLocation(FVector(0, 0, 0));

	//WeaponWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WeaponWidgetComponent");
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
	//WeaponSkeletalMeshComp->PlayAnimation(WeaponFireAnimationAsset, false);
	UE_LOG(LogTemp, Warning, TEXT("inside WeaponBase Fire"));
}

void AWeaponBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("You Overlapped with weapon"));
	if (!WeaponPickedUp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("You Overlapped with weapon"));
		if (OtherActor)
		{
			AMovementCharacter* Char = Cast<AMovementCharacter>(OtherActor);
			if (Char && WeaponInventoryItem)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Did overlap with Ninja Char"));
				//Char->AddToInventory(WeaponInventoryItem);
				Char->PotentialWeapon = this;
				Char->TriggerPickup(GetActorLocation());
				/*Char->Weapon = this;
				Char->TriggerPickup(GetActorLocation());*/
				
				
				
				/*Char->Weapon->AttachToComponent(Char->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightShoulderSocket"));
				WeaponPickedUp = true;*/
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
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("You no longer Overlap with weapon"));
		if (Char)
		{	
			Char->PotentialWeapon = nullptr;
			//Char->Weapon = nullptr;
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

void AWeaponBase::SpawnProjectile(FVector Location, FRotator Rotation)
{
	UWorld* CurrentWorld = GetWorld();
	if (CurrentWorld)
	{
		if (ProjectileToSpawn)
		{
			CurrentWorld->SpawnActor<AProjectile>(ProjectileToSpawn, Location, Rotation);
		}
	}
}



