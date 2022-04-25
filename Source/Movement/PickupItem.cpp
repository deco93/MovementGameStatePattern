// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BandageItem.h"
#include "UObject/ConstructorHelpers.h"
#include "MovementCharacter.h"

// Sets default values
APickupItem::APickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	PickupSMComp = CreateDefaultSubobject<UStaticMeshComponent>("PickupSMComp");
	RootComponent = PickupSMComp;
	PickupBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>("PickupBoxCollisionComponent");
	/*static ConstructorHelpers::FObjectFinder<UBandageItem> BandageItemBP(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/InventoryEquivalents/BP_Bandage.BP_Bandage'"));
	if (BandageItemBP.Succeeded())
	{
		BandageInventoryItem = BandageItemBP.Object;
	}*/
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	PickupBoxCollisionComponent->SetRelativeLocation(FVector(0,0,0));
	PickupBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OnOverlapBegin);
	PickupBoxCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &APickupItem::OnOverlapEnd);
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped with bandage"));
	AMovementCharacter* Player = Cast<AMovementCharacter>(OtherActor);
	if (Player)
	{
		Player->AddToInventory(Player->BandageInventoryItem.GetDefaultObject());
	}
}

void APickupItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap end with bandage"));
}

void APickupItem::Consume()
{

}

void APickupItem::Pickup()
{

}

void APickupItem::Equip()
{

}

void APickupItem::UnEquip()
{

}

bool APickupItem::InHand()
{
	return false;
}

