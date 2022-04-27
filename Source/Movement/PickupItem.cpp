// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BandageItem.h"
#include "UObject/ConstructorHelpers.h"
#include "MovementCharacter.h"
#include "ArmedState.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

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
	
	PlayerCharacter= Cast<AMovementCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
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
		//UBandageItem* UBItem = Cast<UBandageItem>(Player->BandageInventoryItem.GetDefaultObject());
		UBandageItem* UBItem = NewObject<UBandageItem>(this, Player->BandageInventoryItem.Get(), FName("BandageInventoryItem"));
		UBItem->SetMappedItem(this);
		PickupSMComp->SetVisibility(false);
		PickupBoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PickupBoxCollisionComponent->SetGenerateOverlapEvents(false);
		Player->AddToInventory(UBItem);
	}
}

void APickupItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap end with bandage"));
}

void APickupItem::Consume()
{
	UE_LOG(LogTemp, Warning, TEXT("Inside  Consume for PickupItem.cpp/bandage"));
	if (PlayerCharacter->Cuts > 0)
	{
		//play sound of Bandage and then subtract cuts
		if (PlayerCharacter->PlayerAudioComponent && PlayerCharacter->BandageSound && PlayerCharacter->BandageAnimation)
		{
			if (PlayerCharacter->PlayerAudioComponent->IsPlaying())
				PlayerCharacter->PlayerAudioComponent->Stop();
			PlayerCharacter->PlayerAudioComponent->SetSound(PlayerCharacter->BandageSound);
			PlayerCharacter->PlayerAudioComponent->Play();
			PlayerCharacter->PlayAnimMontage(PlayerCharacter->BandageAnimation,1,FName("Default"));
			PlayerCharacter->Cuts -= 1;
			PlayerCharacter->RemoveFromInventory(PlayerCharacter->CurrentInventoryItemInHand);
			GetWorld()->GetTimerManager().SetTimer(BANDAGE_TIMER_HANDLE, this, &APickupItem::OnConsume, 1.5f, false);
		}
	}
}

void APickupItem::Pickup()
{

}

void APickupItem::Equip()
{
	//UE_LOG(LogTemp, Warning, TEXT("inside equip in PickupItem"));
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

void APickupItem::UnEquip()
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

bool APickupItem::InHand()
{
	return false;
}

void APickupItem::DetachFromCharacterSocket()
{
	DetachRootComponentFromParent(false);
}

void APickupItem::OnConsume()
{
	if (PlayerCharacter)
	{
		Cast<AActor>(PlayerCharacter->CurrentPickupItemInHand)->Destroy();
		PlayerCharacter->CurrentPickupItemInHand = nullptr;
	}
}

UStaticMeshComponent* APickupItem::GetStaticMeshComp()
{
	if (PickupSMComp)
		return PickupSMComp;
	return nullptr;
}

UBoxComponent* APickupItem::GetBoxComponent()
{
	if (PickupBoxCollisionComponent)
		return PickupBoxCollisionComponent;
	return nullptr;
}

