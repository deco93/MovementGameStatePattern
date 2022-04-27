// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "SwimmingState.h"
//#include "NinjaState.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "MovementCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickup, FVector, Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAim, bool, IsAim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnSurvivalStatsUIUpdate, int, CutsCount,bool, IsCutsVisible, float, WaterFill, float, FoodFill, float, BloodFill, float, HealthFill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPickup, FVector, Location, FText, WeaponName);

UCLASS(config=Game)
class AMovementCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AMovementCharacter();
	~AMovementCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* Inventory;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UFUNCTION()
	void OnBeginOverlap( UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,  bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	void Jump();

	void Pickup();

	void Aim();

	void StopAim();

	void Fire();

	void StopFire();

	void Use();

	/*void LerpAim(float DeltaSeconds);
	float CurrentLerpDuration = 0.0f;*/
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnPickup OnPickup;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnAim OnAim;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnSurvivalStatsUIUpdate OnSurvivalStatsUIUpdate;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnDeath OnDeathOfCharacter;

protected:

	

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Called for up/down input only for ledge climbing or rope climbing*/
	void MoveUp(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	

	UFUNCTION()
	void OnClimbComplete();

	/*UFUNCTION()
	void OnBandageComplete();*/
	UPROPERTY()
	APlayerController* MovementCharacterPC;
	int SizeX;//Viewport X size
	int SizeY;//Viewport Y size
public:
	//UPROPERTY()
	class SwimmingState* swimmingState;
	//UPROPERTY()
	class ClimbingState* climbingState;
	//UPROPERTY()
	class ArmedState* armedState;
	//UPROPERTY()
	class NinjaState* currentNinjaState;
	UPROPERTY()
	class AMovementGameMode* GM;
	float TraceDistance = 500.0f;
	FVector2D ScreenMiddleCoordinates;
	FVector CrosshairProjectedWorldLocation;
	FVector CrosshairProjectedWorldDirection;
	FVector AimDirection;

	//UPROPERTY()
	class IPickupInterface* CurrentPickupItemInHand = nullptr;
	UPROPERTY()
	class UItem* CurrentInventoryItemInHand = nullptr;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void SetTimer();
	UFUNCTION(BlueprintCallable)
	bool GetClimbUp();

	UFUNCTION(BlueprintCallable)
	float GetLedgeHorizontalSpeed();

	UFUNCTION(BlueprintCallable)
	bool IsArmedWithGun();

	UFUNCTION(BlueprintCallable)
	bool IsAiming();

	UPROPERTY(EditAnywhere)
	USoundCue* JumpSound;

	UPROPERTY(EditAnywhere)
	USoundCue* ClimbSound;

	UPROPERTY(EditAnywhere)
	USoundCue* SwimSound;

	UPROPERTY(EditAnywhere)
	USoundCue* M4Sound;

	UPROPERTY(EditAnywhere)
	USoundCue* ASValSound;

	UPROPERTY(EditAnywhere)
	USoundCue* BandageSound;

	UPROPERTY(EditAnywhere)
	USoundCue* WaterSound;

	UPROPERTY(EditAnywhere)
	USoundCue* AppleEatingSound;

	UPROPERTY(EditAnywhere)
	USoundCue* NinjaHitSound;

	UPROPERTY(EditAnywhere)
	class UAudioComponent* PlayerAudioComponent;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* BandageAnimation;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* WaterBottleAnimation;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* TakeDamageHitAnimation;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SurvivalStats")
	int Cuts;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SurvivalStats")
	float Water;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SurvivalStats")
	float Food;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SurvivalStats")
	float Blood;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "SurvivalStats")
	float Health;





	UFUNCTION(BlueprintCallable, Category = "Items")
	void UseItem(class UItem* Item);

	/*UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeaponBase> WeaponClass;*/

	UFUNCTION()
	void AddToInventory(class UItem* Item);

	UFUNCTION()
	void RemoveFromInventory(class UItem* Item);
	
	//used when pickup prompt shown and later swapped with Weapon if 
	//player already has a Weapon after existing weapon dropped else just assigned to Weapon
	UPROPERTY()
	class AWeaponBase* PotentialWeapon;
	UPROPERTY()
	class AWeaponBase* Weapon;

	UFUNCTION()
	void TriggerPickup(FVector Location);

	UFUNCTION()
	void TriggerAimStatus(bool IsAim);


	UFUNCTION()
	void TakeDamage();

	UFUNCTION()
	void OnTakingDamage();

	FVector GetCrosshairProjectedWorldLocation();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UItem> BandageInventoryItem;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UItem> WaterBottleInventoryItem;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UItem> FoodInventoryItem;


	FTimerHandle CHARACTER_TIMER_HANDLE;
	/*bool WeaponInHand = false;
	void EquipWeaponToHand();*/
};

