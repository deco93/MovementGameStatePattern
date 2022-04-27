// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupInterface.h"
#include "PickupItem.generated.h"

UCLASS()
class MOVEMENT_API APickupItem : public AActor, public IPickupInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* PickupSMComp;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* PickupBoxCollisionComponent;

	/*UPROPERTY(EditAnywhere)
	class UBandageItem*  BandageInventoryItem;*/
	UPROPERTY()
	class AMovementCharacter* PlayerCharacter;

	void Consume() override;
	void Pickup() override;
	void Equip() override;
	void UnEquip() override;
	bool InHand() override;
	void DetachFromCharacterSocket() override;
	UFUNCTION()
	void OnConsume();
	FTimerHandle BANDAGE_TIMER_HANDLE;

	class UStaticMeshComponent* GetStaticMeshComp() override;
	class UBoxComponent* GetBoxComponent() override;
};
