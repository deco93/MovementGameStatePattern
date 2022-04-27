// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupWater.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "WaterItem.h"
#include "ArmedState.h"

// Sets default values
APickupWater::APickupWater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	PickupSMComp = CreateDefaultSubobject<UStaticMeshComponent>("PickupSMComp");
	RootComponent = PickupSMComp;
	PickupBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>("PickupBoxCollisionComponent");
}

// Called when the game starts or when spawned
void APickupWater::BeginPlay()
{
	Super::BeginPlay();
	PickupBoxCollisionComponent->SetRelativeLocation(FVector(0, 0, 0));
	PickupBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupWater::OnOverlapBegin);
	PickupBoxCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &APickupWater::OnOverlapEnd);
	
	PlayerCharacter = Cast<AMovementCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void APickupWater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupWater::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap with BOTTLE"));
	AMovementCharacter* Player = Cast<AMovementCharacter>(OtherActor);
	if (Player)
	{
		UWaterItem* UBItem = Cast<UWaterItem>(Player->WaterBottleInventoryItem.GetDefaultObject());
		UBItem->SetMappedItem(this);
		PickupSMComp->SetVisibility(false);
		PickupBoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PickupBoxCollisionComponent->SetGenerateOverlapEvents(false);
		Player->AddToInventory(UBItem);
	}
}

void APickupWater::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap end with BOTTLE"));
}

void APickupWater::Consume()
{
	if (PlayerCharacter->Water < 100.0f)
	{
		//play sound of Bandage and then subtract cuts
		if (PlayerCharacter->PlayerAudioComponent && PlayerCharacter->WaterSound && PlayerCharacter->WaterBottleAnimation)
		{
			if (PlayerCharacter->PlayerAudioComponent->IsPlaying())
				PlayerCharacter->PlayerAudioComponent->Stop();
			PlayerCharacter->PlayerAudioComponent->SetSound(PlayerCharacter->WaterSound);
			PlayerCharacter->PlayerAudioComponent->Play();
			PlayerCharacter->PlayAnimMontage(PlayerCharacter->WaterBottleAnimation, 4, FName("Default"));
			PlayerCharacter->Water += 35.0f;
			PlayerCharacter->RemoveFromInventory(PlayerCharacter->CurrentInventoryItemInHand);
			GetWorld()->GetTimerManager().SetTimer(WATER_BOTTLE_TIMER_HANDLE, this, &APickupWater::OnConsume, 1.0f, false);
		}
	}
}

void APickupWater::Pickup()
{

}

void APickupWater::Equip()
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

void APickupWater::UnEquip()
{
	/*if (PlayerCharacter && PlayerCharacter->CurrentPickupItemInHand)//means current item in hand is same as the bandage being tried to Use
	{
		PlayerCharacter->CurrentPickupItemInHand->GetStaticMeshComp()->DetachFromParent();
	}*/
		PlayerCharacter->CurrentPickupItemInHand->DetachFromCharacterSocket();
		PlayerCharacter->CurrentPickupItemInHand = nullptr;
}

bool APickupWater::InHand()
{
	return false;
}

void APickupWater::DetachFromCharacterSocket()
{
	DetachRootComponentFromParent(false);
}

void APickupWater::OnConsume()
{
	if (PlayerCharacter)
	{
		Cast<AActor>(PlayerCharacter->CurrentPickupItemInHand)->Destroy();
		PlayerCharacter->CurrentPickupItemInHand = nullptr;
	}
}

UStaticMeshComponent* APickupWater::GetStaticMeshComp()
{
	if (PickupSMComp)
		return PickupSMComp;
	return nullptr;
}

UBoxComponent* APickupWater::GetBoxComponent()
{
	if (PickupBoxCollisionComponent)
		return PickupBoxCollisionComponent;
	return nullptr;
}
