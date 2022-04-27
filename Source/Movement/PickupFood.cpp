// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupFood.h"
#include "FoodItem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MovementCharacter.h"
#include "ArmedState.h"
#include "Components/AudioComponent.h"

// Sets default values
APickupFood::APickupFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	PickupSMComp = CreateDefaultSubobject<UStaticMeshComponent>("PickupSMComp");
	RootComponent = PickupSMComp;
	PickupBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>("PickupBoxCollisionComponent");
}

// Called when the game starts or when spawned
void APickupFood::BeginPlay()
{
	Super::BeginPlay();
	PickupBoxCollisionComponent->SetRelativeLocation(FVector(0, 0, 0));
	PickupBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupFood::OnOverlapBegin);
	PickupBoxCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &APickupFood::OnOverlapEnd);

	PlayerCharacter = Cast<AMovementCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void APickupFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupFood::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap with Food"));
	AMovementCharacter* Player = Cast<AMovementCharacter>(OtherActor);
	if (Player)
	{
		//UFoodItem* UBItem = Cast<UFoodItem>(Player->FoodInventoryItem.GetDefaultObject());
		UFoodItem* UBItem = NewObject<UFoodItem>(this, Player->FoodInventoryItem.Get(), FName("FoodInventoryItem"));
		UBItem->SetMappedItem(this);
		PickupSMComp->SetVisibility(false);
		PickupBoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PickupBoxCollisionComponent->SetGenerateOverlapEvents(false);
		Player->AddToInventory(UBItem);
	}
}

void APickupFood::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap end with Food"));
}

void APickupFood::Consume()
{
	if (PlayerCharacter->Food < 100.0f)
	{
		
		if (PlayerCharacter->PlayerAudioComponent && PlayerCharacter->AppleEatingSound && PlayerCharacter->WaterBottleAnimation)//using same animation fr now for eating too
		{
			if (PlayerCharacter->PlayerAudioComponent->IsPlaying())
				PlayerCharacter->PlayerAudioComponent->Stop();
			PlayerCharacter->PlayerAudioComponent->SetSound(PlayerCharacter->AppleEatingSound);
			PlayerCharacter->PlayerAudioComponent->Play();
			PlayerCharacter->PlayAnimMontage(PlayerCharacter->WaterBottleAnimation, 4, FName("Default"));
			PlayerCharacter->Food += 35.0f;
			PlayerCharacter->RemoveFromInventory(PlayerCharacter->CurrentInventoryItemInHand);
			GetWorld()->GetTimerManager().SetTimer(FOOD_TIMER_HANDLE, this, &APickupFood::OnConsume, 1.0f, false);
		}
	}
}

void APickupFood::Pickup()
{
}

void APickupFood::Equip()
{
	if (PlayerCharacter)
	{
		//UE_LOG(LogTemp, Warning, TEXT("inside equip in PickupItem PlayCharacter init___"));
		if (PlayerCharacter->armedState && !PlayerCharacter->armedState->WeaponInHand)
		{
			PickupSMComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PickupBoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PickupBoxCollisionComponent->SetGenerateOverlapEvents(false);
			PickupSMComp->SetVisibility(true);
			PickupSMComp->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandSocket"));
			PlayerCharacter->CurrentPickupItemInHand = this;
			//UE_LOG(LogTemp, Warning, TEXT("inside equip without weapon in hand"));
		}
		else if (PlayerCharacter->armedState && PlayerCharacter->armedState->WeaponInHand)
		{
			PlayerCharacter->armedState->EquipWeaponToHand(PlayerCharacter);//this will unequip existing weapon in hand
			PickupSMComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PickupBoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PickupBoxCollisionComponent->SetGenerateOverlapEvents(false);
			PickupSMComp->SetVisibility(true);
			PickupSMComp->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandSocket"));
			PlayerCharacter->CurrentPickupItemInHand = this;
			//UE_LOG(LogTemp, Warning, TEXT("inside equip with weapon in hand"));
		}
	}
}

void APickupFood::UnEquip()
{
	if (PlayerCharacter && PlayerCharacter->CurrentPickupItemInHand)//means current item in hand is same as the bandage being tried to Use
	{
		/*if (PlayerCharacter->CurrentPickupItemInHand->GetStaticMeshComp())
		{
			PlayerCharacter->CurrentPickupItemInHand->GetStaticMeshComp()->DetachFromParent();
		}*/
		PlayerCharacter->CurrentPickupItemInHand->DetachFromCharacterSocket();
		PlayerCharacter->CurrentPickupItemInHand = nullptr;
	}
}

bool APickupFood::InHand()
{
	return false;
}

void APickupFood::DetachFromCharacterSocket()
{
	DetachRootComponentFromParent(false);
}

void APickupFood::OnConsume()
{
	if (PlayerCharacter)
	{
		Cast<AActor>(PlayerCharacter->CurrentPickupItemInHand)->Destroy();
		PlayerCharacter->CurrentPickupItemInHand = nullptr;
	}
}

UStaticMeshComponent* APickupFood::GetStaticMeshComp()
{
	return nullptr;
}

UBoxComponent* APickupFood::GetBoxComponent()
{
	return nullptr;
}
