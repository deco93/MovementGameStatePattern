// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "SwimmingState.h"
//#include "NinjaState.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "InputAction.h"
#include "MovementCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickup, FVector, Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAim, bool, IsAim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnSurvivalStatsUIUpdate, int, CutsCount,bool, IsCutsVisible, float, WaterFill, float, FoodFill, float, BloodFill, float, HealthFill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPickup, FVector, Location, FText, WeaponName);

enum InputTypeDirection {
	UP_ITD,
	RIGHT_ITD,
	FORWARD_ITD
};

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))	
	class USpotLightComponent* FlashLight;

	FTimerHandle TIMER_HANDLE_TIME_TO_AIM;
	FTimerHandle TIMER_FIRE_RATE;
	float SecondsBetweenBullet;
	void TimeToAimElapsed();
	void SpawnProjectile(FVector MuzzleLocation);
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

	double LastFireTimestampMillis = 0.0f;

	UFUNCTION()
	void OnBeginOverlap( UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,  bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	void Pickup();
	UFUNCTION(Server, Reliable)
	void Server_Pickup();
	UFUNCTION(Client, Reliable)
	void Client_AddToInventory(class UGunItem* GunItem);

	UFUNCTION(Client, Reliable)
	void Client_RemoveFromInventory(class UGunItem* GunItem);

	UFUNCTION(Server, Reliable)
	void Server_ArmedStateSetWeaponInHand(bool bIsWeaponInHand);

	UFUNCTION(Server, Reliable)
	void Server_ArmedStateSetIsAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void Server_ArmedStateSetIsFiring(bool bIsFiring);

	UFUNCTION(Server, Reliable)
	void Server_ArmedStateSetIsFireReady(bool bIsFireReady);

	UFUNCTION(Server, Reliable)
	void Server_SwimmingStateSetInWater(bool bIsInWater);

	UFUNCTION(Server, Reliable)
	void Server_ClimbingStateSetClimbUp(bool bIsClimbUp);

	UFUNCTION(Server, UnReliable, WithValidation)
	void Server_ClimbingStateSetLedgeHSpeed(float LedgeHSpeed);


	UFUNCTION(Server, Reliable)
	void Server_Fire(FVector MuzzleLocation);

	UFUNCTION(Client, Reliable)
	void Client_DrawFireSphere(FVector MuzzleLocation, FVector ProjectileSpawnLocation);

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

	//bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

protected:


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

	void SetClimbTimer();
	
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
	class UTextRenderComponent* CharacterNameComponent;

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

	UFUNCTION(Server, Reliable)
	void Server_UseItem(class UItem* Item);

	/*UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeaponBase> WeaponClass;*/

	UFUNCTION()
	void AddToInventory(class UItem* Item);

	UFUNCTION()
	void RemoveFromInventory(class UItem* Item);
	
	//used when pickup prompt shown and later swapped with Weapon if 
	//player already has a Weapon after existing weapon dropped else just assigned to Weapon
	UPROPERTY(Replicated)
	class AWeaponBase* PotentialWeapon;
	//UPROPERTY(Replicated)
	UPROPERTY(Replicated)
	class AWeaponBase* Weapon;

	UFUNCTION()
	void TriggerPickup(FVector Location);

	UFUNCTION()
	void TriggerAimStatus(bool IsAim);


	UFUNCTION()
	void TakeDamage();

	UFUNCTION()
	void OnTakingDamage();

	void EnhancedInputJump();

	void EnhancedInputMoveForward(const FInputActionValue& Value);
	void EnhancedInputMoveRight(const FInputActionValue& Value);
	void EnhancedInputSwimUp(const FInputActionValue& Value);

	void ToggleFlashLight();
	//### BEGIN movement methods ###
	//ArmedState
	void EquipWeaponToHand();
	//SwimmingState
	void HandleSwimmingStateInput(InputTypeDirection inputType, float axisValue);
	void UpdateSwimmingState();
	//ClimbingState
	void HandleClimbingStateInput(InputTypeDirection inputType, float axisValue);
	void UpdateClimbingState();
	//### END movement methods ###

	FVector GetCrosshairProjectedWorldLocation();
	//### BEGIN movement related props ###
	//1. ArmedState vars
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool WeaponInHand = false;//for managing animation states
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsAiming = false;//for managing animation states
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsFiring = false;//for managing animation states
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsFireReady = false;//for managing when fire is actually possible after an initial delay to aim

	//2. ClimbingState vars
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool ClimbUp = false;
	FVector LastLedgeClimbPosition;
	UPROPERTY(Replicated, BlueprintReadOnly)
	float LedgeHorizontalSpeed = 0.0f;

	//3. SwimmingState vars
	float WaterZ = 0.0f;
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool InWater = false;
	float SwimStateSoundCooldown = 0.0f;
	//### BEGIN movement related props ###

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UItem> BandageInventoryItem;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UItem> WaterBottleInventoryItem;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UItem> FoodInventoryItem;


	FTimerHandle CHARACTER_TIMER_HANDLE;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enhanced Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* InputToJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* InputToToggleFlashLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* InputToPickup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* InputToUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* InputToAim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* InputToFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* InputToMoveForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* InputToMoveRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* InputToSwimUp;

};

