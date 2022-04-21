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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnSurvivalStatsUIUpdate, bool, IsCutsVisible, float, WaterFill, float, FoodFill, float, BloodFill, float, HealthFill);

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* Inventory;
public:
	AMovementCharacter();
	~AMovementCharacter();

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

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnPickup OnPickup;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnAim OnAim;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnSurvivalStatsUIUpdate OnSurvivalStatsUIUpdate;

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

	APlayerController* MovementCharacterPC;
	int SizeX;//Viewport X size
	int SizeY;//Viewport Y size
public:
	class SwimmingState* swimmingState;
	class ClimbingState* climbingState;
	class ArmedState* armedState;
	class NinjaState* currentNinjaState;
	class AMovementGameMode* GM;
	float TraceDistance = 500.0f;
	FVector2D ScreenMiddleCoordinates;
	FVector CrosshairProjectedWorldLocation;
	FVector CrosshairProjectedWorldDirection;
	FVector AimDirection;

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
	class AWeaponBase* PotentialWeapon;

	class AWeaponBase* Weapon;

	UFUNCTION()
	void TriggerPickup(FVector Location);

	UFUNCTION()
	void TriggerAimStatus(bool IsAim);

	FVector GetCrosshairProjectedWorldLocation();
	/*bool WeaponInHand = false;
	void EquipWeaponToHand();*/
};

