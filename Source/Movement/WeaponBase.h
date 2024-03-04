// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunItem.h"
#include "WeaponBase.generated.h"

UCLASS()
class MOVEMENT_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* WeaponSkeletalMeshComp;

	UPROPERTY(EditAnywhere)
	class UAnimationAsset* WeaponFireAnimationAsset;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* WeaponBoxCollisionComponent;

	

	UPROPERTY(EditAnywhere)
	class UGunItem* WeaponInventoryItem;
	/*UPROPERTY(EditAnywhere)
	TSubclassOf<class UGunItem> WeaponInventoryItem;*/

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileToSpawn;

	UPROPERTY(EditAnywhere)
	float TimeToAim = 0.0f;

	UPROPERTY(EditAnywhere)
	int WeaponFireRate = 900;//projectiles per minute

	int Durability = 30;
	
	void Fire();

	bool WeaponPickedUp = false;//set once player picks up weapon so a prompt to pickup is not shown anymore

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UGunItem* GetWeaponInventoryItem();

	void SpawnProjectile(FVector Location, FRotator Rotation);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
