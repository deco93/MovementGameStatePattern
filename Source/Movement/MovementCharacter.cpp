// Copyright Epic Games, Inc. All Rights Reserved.

#include "MovementCharacter.h"
//#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "SwimmingState.h"
#include "ClimbingState.h"
#include "ArmedState.h"
#include "NinjaState.h"
#include "MovementGameMode.h"
#include "ZombieNPC.h"
#include "Item.h"
#include "InventoryComponent.h"
#include "Components/AudioComponent.h"
#include "WeaponBase.h"
#include "PickupInterface.h"
//#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "Components/SpotLightComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Misc/Paths.h"
#include "DrawDebugHelpers.h"
#include "CommonUtils.h"
#include <chrono>


//////////////////////////////////////////////////////////////////////////
// AMovementCharacter

void AMovementCharacter::TimeToAimElapsed()
{
	UE_LOG(LogTemp, Warning, TEXT("called %s"), ANSI_TO_TCHAR(__FUNCTION__));
	if (armedState)
	{
		armedState->IsFireReady = true;
		FName GunMuzzleName = FName(FString("b_gun_muzzleflash"));
		FVector MuzzleLocation = Weapon->WeaponSkeletalMeshComp->GetBoneLocation(GunMuzzleName);
		DrawDebugLine(GetWorld(), MuzzleLocation, MuzzleLocation + (GetActorUpVector() * 500.0f), FColor::Cyan, false, 10.0f);
	}
}

AMovementCharacter::AMovementCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("CharacterInventory"));
	Inventory->Capacity = 20;


	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLight"));
	FlashLight->SetupAttachment(GetMesh());
	FlashLight->SetVisibility(false);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMovementCharacter::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMovementCharacter::OnEndOverlap);

	PlayerAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PlayerAudioComponent"));
	PlayerAudioComponent->SetupAttachment(RootComponent);

	CharacterNameComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CharacterNameComponent"));
	/*FAttachmentTransformRules DefaultRules= FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	CharacterName->AttachToComponent(RootComponent, DefaultRules);*/
	if (CharacterNameComponent)
	{	
		CharacterNameComponent->SetText(FText::FromString("TestName"));
		CharacterNameComponent->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> M4SoundCueClass(TEXT("SoundCue'/Game/Sounds/guns/M4_gun_shot_loop_Cue.M4_gun_shot_loop_Cue'"));
	if (M4SoundCueClass.Succeeded())
	{
		M4Sound = M4SoundCueClass.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> ASValSoundCueClass(TEXT("SoundCue'/Game/Sounds/guns/asval_gunshot_loop_Cue.asval_gunshot_loop_Cue'"));
	if (ASValSoundCueClass.Succeeded())
	{
		ASValSound = ASValSoundCueClass.Object;
	}
	swimmingState = new SwimmingState();
	climbingState = new ClimbingState();
	armedState = new ArmedState();
	Water = 100.f;
	Food = 100.f;
	Blood = 100.0f;
	Health = 100.0f;
	Cuts = 0;
}

AMovementCharacter::~AMovementCharacter()
{
	if (swimmingState)
		delete swimmingState;

	if (climbingState)
		delete climbingState;

	if (armedState)
		delete armedState;
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMovementCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);


	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMovementCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMovementCharacter::LookUpAtRate);

	//Enhanced Input handling
	APlayerController* PC = Cast<APlayerController>(GetController());
	if(UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		InputSystem->ClearAllMappings();
		InputSystem->AddMappingContext(InputMappingContext, 0);
	}
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(InputToJump, ETriggerEvent::Triggered, this, &AMovementCharacter::EnhancedInputJump);

	Input->BindAction(InputToToggleFlashLight, ETriggerEvent::Triggered, this, &AMovementCharacter::ToggleFlashLight);
	Input->BindAction(InputToPickup, ETriggerEvent::Triggered, this, &AMovementCharacter::Pickup);
	Input->BindAction(InputToUse, ETriggerEvent::Triggered, this, &AMovementCharacter::Use);

	Input->BindAction(InputToAim, ETriggerEvent::Triggered, this, &AMovementCharacter::Aim);
	Input->BindAction(InputToAim, ETriggerEvent::Completed, this, &AMovementCharacter::StopAim);

	Input->BindAction(InputToFire, ETriggerEvent::Triggered, this, &AMovementCharacter::Fire);
	Input->BindAction(InputToFire, ETriggerEvent::Completed, this, &AMovementCharacter::StopFire);

	Input->BindAction(InputToMoveForward, ETriggerEvent::Triggered, this, &AMovementCharacter::EnhancedInputMoveForward);
	Input->BindAction(InputToMoveRight, ETriggerEvent::Triggered, this, &AMovementCharacter::EnhancedInputMoveRight);
	Input->BindAction(InputToSwimUp, ETriggerEvent::Triggered, this, &AMovementCharacter::EnhancedInputSwimUp);
}


void AMovementCharacter::OnBeginOverlap( UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (OtherActor->ActorHasTag("Lake") && swimmingState)
	{
		swimmingState->InWater = true;
		swimmingState->WaterZ = GetActorLocation().Z;
		UE_LOG(LogTemp, Warning, TEXT("Overlap begin with lake Z location: %f"), swimmingState->WaterZ);
		//UE_LOG(LogTemp, Warning, TEXT("Overlap begin with lake"));
	}
}

void AMovementCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Lake") && swimmingState)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap end with lake"));
		swimmingState->InWater = false;
		swimmingState->WaterZ = 0.0f;
	}

}

void AMovementCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("inside beginplay of character"));
	GM = Cast<AMovementGameMode>(GetWorld()->GetAuthGameMode());
	MovementCharacterPC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//InitUpVector = GetActorUpVector();
	SizeX = 0, 
	SizeY = 0;
	MovementCharacterPC->GetViewportSize(SizeX, SizeY);
	ScreenMiddleCoordinates.X = SizeX / 2.0f;
	ScreenMiddleCoordinates.Y = SizeY / 2.0f;
	if(GM)
		GM->SetPlayerCharacter(this);
	/*if (WeaponClass)
	{
		FActorSpawnParameters WeaponSpawnParams;
		WeaponSpawnParams.bNoFail = true;
		WeaponSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform WeaponTransform;
		WeaponTransform.SetLocation(FVector::ZeroVector);
		WeaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));

		Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, WeaponTransform, WeaponSpawnParams);
		if (Weapon)
		{
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightShoulderSocket"));
		}
	}*/
	bool IsReadSuccess = false;
	FString ReadOutputMessage;
	FString ReadString = CommonUtils::ReadFile(FPaths::ProjectConfigDir() + "/DefaultSpawns.json", IsReadSuccess, ReadOutputMessage);

	if (IsReadSuccess)
	{
		GEngine->AddOnScreenDebugMessage(-1,20.0f, FColor::Blue, FString::Printf(TEXT("file read success contents: %s"), *ReadString));
		TArray<TSharedPtr<FJsonValue>> ReadJsonArr;
		if (FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(ReadString), ReadJsonArr))
		{
			if (ReadJsonArr[0].IsValid())
			{
				FString CharacterName;
				if (const TSharedPtr<FJsonObject> ReadObj = ReadJsonArr[0]->AsObject())
				{
					if (ReadObj->TryGetStringField("ItemClass", CharacterName))
					{
						if (CharacterNameComponent)
							CharacterNameComponent->SetText(FText::FromString(CharacterName));
					}
				}
			}
		}
	}
	else
		GEngine->AddOnScreenDebugMessage(-1,20.0f, FColor::Blue, FString::Printf(TEXT("file read failed")));
}


void AMovementCharacter::Tick(float DeltaSeconds)
{
	//UE_LOG(LogTemp, Warning, TEXT("inside char tick %d tick delta %f"), count++, DeltaSeconds);
	Water -= 1.0f * DeltaSeconds;
	Food -= 1.0f * DeltaSeconds;
	Water = FMath::Clamp(Water,0.0f,100.0f);
	Food = FMath::Clamp(Food,0.0f,100.0f);
	Cuts = FMath::Clamp(Cuts, 0, 3);

	if (Water < 25.0f || Food < 25.0f || Blood < 20.0f)
	{
		Health -= 1.0f * DeltaSeconds;
		Health = FMath::Clamp(Health, 0.0f, 100.0f);
	}
	else if(Health <100.0f)
	{
		Health += 1.0f * DeltaSeconds;
		Health = FMath::Clamp(Health, 0.0f, 100.0f);
	}
	if (Cuts > 0)
	{
		Blood -= 1.0f * DeltaSeconds*Cuts;
		Blood = FMath::Clamp(Blood, 0.0f, 100.0f);
	}
	else if (Water >= 25.0f && Food >= 25.0f && Blood <100.0f)
	{
		Blood += 1.0f * DeltaSeconds*0.5f;
		Blood = FMath::Clamp(Blood, 0.0f, 100.0f);
	}
	//lerp aim
	//LerpAim(DeltaSeconds);
	OnSurvivalStatsUIUpdate.Broadcast(Cuts,Cuts>0, Water/100.0f, Food/100.0f, Blood/100.0f, Health/100.f);
	if(Health<=0.0f)
		OnDeathOfCharacter.Broadcast();
	if (swimmingState && swimmingState->InWater)
	{
		float CapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
		//UE_LOG(LogTemp, Warning, TEXT("Half Capsule height: %f"), CapsuleHalfHeight);
		float HeightBelowWater = swimmingState->WaterZ - GetActorLocation().Z;
		UCharacterMovementComponent* uCharaceterMovement = GetCharacterMovement();
		if (uCharaceterMovement->MovementMode != EMovementMode::MOVE_Swimming && HeightBelowWater > CapsuleHalfHeight)
		{
			//TODO set swimming state
			uCharaceterMovement->SetMovementMode(EMovementMode::MOVE_Swimming);
			currentNinjaState = swimmingState;
			//currentNinjaState->HandleInput(this, InputTypeDirection::UP_ITD, 1.0f);
		}
		else if (HeightBelowWater <= CapsuleHalfHeight && uCharaceterMovement->MovementMode != EMovementMode::MOVE_Falling)
		{
			uCharaceterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
		}

		if (currentNinjaState && uCharaceterMovement->MovementMode == EMovementMode::MOVE_Swimming)
			currentNinjaState->Update(this);
		//UE_LOG(LogTemp, Warning, TEXT("current movement mode walking: %d swimming: %d"), uCharaceterMovement->MovementMode == EMovementMode::MOVE_Walking, uCharaceterMovement->MovementMode == EMovementMode::MOVE_Swimming);
	}
	if (GetCharacterMovement()->IsFalling())
	{
		if (GM)
		{
			FVector StartLoc = GetActorLocation() + GetActorForwardVector() * 35.0f;
			FHitResult OHit =GM->DrawLineTrace(GetActorLocation()+GetActorForwardVector()*35.0f, StartLoc + (GetActorForwardVector()* TraceDistance));
			FHitResult OHit2;
			if (OHit.bBlockingHit && OHit.GetActor()->ActorHasTag("Ledge"))
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit success on ledge"));
				FVector SecondTraceDest = OHit.ImpactPoint + GetActorForwardVector() * 10.0f;
				FVector SecondTraceSource = FVector(SecondTraceDest.X, SecondTraceDest.Y, SecondTraceDest.Z + 200.0f);
				//get location above ledge where we need to stand after climb animation
				OHit2 = GM->DrawLineTrace(SecondTraceSource, SecondTraceDest);
				FVector LocationToHang(OHit.ImpactPoint.X, OHit.ImpactPoint.Y, OHit2.ImpactPoint.Z - (1.5f*GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
				GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);


				currentNinjaState = climbingState;

				SetActorLocation(LocationToHang);
				GetCharacterMovement()->StopMovementImmediately();
				GetCharacterMovement()->BrakingDecelerationFlying = 1200.0f;
				GetCharacterMovement()->MaxFlySpeed = 100.0f;
				SetActorRotation((OHit.ImpactNormal*-1).Rotation());
				GetCharacterMovement()->RotationRate = FRotator(0,0,0);
				climbingState->LastLedgeClimbPosition = OHit2.ImpactPoint + GetActorUpVector() * (GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() + 20.0f);
				//LastLedgeClimbPosition = OHit2.ImpactPoint + GetActorUpVector() * (GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() + 20.0f);
				
			}
			/*else if(OHit.bBlockingHit && !OHit.Actor->ActorHasTag("Ledge"))
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit success"));
			}
			else 
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit failed"));
			}*/
		}
	}
	if (armedState && armedState->WeaponInHand && armedState->IsAiming)
	{
		GetCrosshairProjectedWorldLocation();
		const FVector CameraLookDirectionFarPoint = (CrosshairProjectedWorldLocation + (CrosshairProjectedWorldLocation - MovementCharacterPC->PlayerCameraManager->GetCameraLocation()).GetSafeNormal() * 1000.f);

		AimDirection = (CameraLookDirectionFarPoint - GetActorLocation()).GetSafeNormal();
		SetActorRotation(AimDirection.Rotation());
	}
	if (armedState && armedState->WeaponInHand && armedState->IsAiming && armedState->IsFiring && !PlayerAudioComponent->IsPlaying())
	{
		if (Weapon->Durability <= 0)
		{
			Weapon->DetachRootComponentFromParent(false);
			RemoveFromInventory(Weapon->WeaponInventoryItem);
			Weapon->Destroy();
			Weapon = nullptr;
			StopAim();
			armedState->WeaponInHand = false;
			armedState->IsFiring = false;
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("Trying to play fire sound"));
		PlayerAudioComponent->Play();
		Weapon->Durability -= 1;
		GetCrosshairProjectedWorldLocation();
		UE_LOG(LogTemp, Warning, TEXT("Current Crosshair world loc: (%f,%f,%f)"), CrosshairProjectedWorldLocation.X, CrosshairProjectedWorldLocation.Y, CrosshairProjectedWorldLocation.Z);
		AimDirection = CrosshairProjectedWorldLocation - MovementCharacterPC->PlayerCameraManager->GetCameraLocation();
		if (GM)
		{
			FHitResult HResult = GM->DrawLineTraceForFiring(CrosshairProjectedWorldLocation, CrosshairProjectedWorldLocation + AimDirection * 1000.0f, FColor::Red, false, 10.0f);
			if (HResult.bBlockingHit)
			{
				AZombieNPC* Zombie = Cast<AZombieNPC>(HResult.GetActor());
				if (Zombie)
				{
					//Zombie->TakeDamage(50.0f);
					Zombie->TakeDamage(Weapon->WeaponInventoryItem->Damage);
					UE_LOG(LogTemp, Warning, TEXT("---Zombie Hit--- Health now: %f"), Zombie->Health);
				}
			}
		}
	}
}

void AMovementCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMovementCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


void AMovementCharacter::OnClimbComplete()
{
	UE_LOG(LogTemp, Warning,TEXT("ClimbUp finished"));
	climbingState->ClimbUp = false;
	//ClimbUp = false;
	if(climbingState)
		SetActorLocation(climbingState->LastLedgeClimbPosition);
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}



void AMovementCharacter::SetTimer()
{
	if (currentNinjaState)
	{
		GetWorld()->GetTimerManager().SetTimer(GM->TIMER_HANDLE, this, &AMovementCharacter::OnClimbComplete, 2.0f, false);
	}
}

bool  AMovementCharacter::GetClimbUp()
{

	return climbingState->ClimbUp;
}

float AMovementCharacter::GetLedgeHorizontalSpeed()
{
	return climbingState->LedgeHorizontalSpeed;
}

bool AMovementCharacter::IsArmedWithGun()
{
	if (armedState)
	{
		return armedState->WeaponInHand;
	}
	return false;
}

bool AMovementCharacter::IsAiming()
{
	if (armedState)
	{
		return armedState->WeaponInHand && armedState->IsAiming;
	}
	return false;
}

void AMovementCharacter::UseItem(class UItem* Item)
{
	if (Item)
	{
		Item->Use(this);
		//Item->OnUse(this);//this is bp implementation
	}
}

void AMovementCharacter::AddToInventory(UItem* Item)
{
	UE_LOG(LogTemp, Warning, TEXT("MovementCharacter AddToInventory called"));
	if (Inventory)
	{
		Inventory->AddItem(Item);
	}
}

void AMovementCharacter::RemoveFromInventory(class UItem* Item)
{
	UE_LOG(LogTemp, Warning, TEXT("MovementCharacter RemoveFromInventory called"));
	if (Inventory)
	{
		Inventory->RemoveItem(Item);
	}
}

void AMovementCharacter::TriggerPickup(FVector Location)
{
	OnPickup.Broadcast(Location);
}

void AMovementCharacter::TriggerAimStatus(bool IsAim)
{
	OnAim.Broadcast(IsAim);
}

void AMovementCharacter::TakeDamage()
{
	
		GetWorld()->GetTimerManager().SetTimer(CHARACTER_TIMER_HANDLE, this, &AMovementCharacter::OnTakingDamage, 1.4f, false);
	
}

void AMovementCharacter::OnTakingDamage()
{
	if (TakeDamageHitAnimation && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(TakeDamageHitAnimation))
	{
		UE_LOG(LogTemp, Warning, TEXT("NinjaHit montage played..."));
		PlayAnimMontage(TakeDamageHitAnimation, 2.0f, FName("Default"));
		Health -= 5.0f;
		Cuts++;
		//Cuts = FMath::Clamp(Cuts, 0, 4);
		if (PlayerAudioComponent && NinjaHitSound)
		{
			if (PlayerAudioComponent->IsPlaying())
				PlayerAudioComponent->Stop();
			PlayerAudioComponent->SetSound(NinjaHitSound);
			PlayerAudioComponent->Play();
		}
	}
}

void AMovementCharacter::EnhancedInputJump()
{	
	if (!GetCharacterMovement()->IsFalling())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), JumpSound);
		if (armedState)
			StopAim();
	}
	ACharacter::Jump();
}

void AMovementCharacter::EnhancedInputMoveForward(const FInputActionValue& Value)
{
	float ValueForward = Value.Get<float>();
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying)
		UE_LOG(LogTemp, Warning, TEXT("Value in move forward %f"), ValueForward);

	if ((Controller != nullptr) && (ValueForward != 0.0f) && (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Flying))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, ValueForward);
	}
	else if ((Controller != nullptr) && (ValueForward != 0.0f) && (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying) && !climbingState->ClimbUp)
	{
		//else if ((Controller != nullptr) && (Value != 0.0f) && (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying) && !ClimbUp)
		climbingState->ClimbUp = true;
		if (currentNinjaState)
			currentNinjaState->HandleInput(this, InputTypeDirection::FORWARD_ITD, ValueForward);
		//ClimbUp = true;
		GetWorld()->GetTimerManager().SetTimer(GM->TIMER_HANDLE, this, &AMovementCharacter::OnClimbComplete, 2.0f, false);
	}
}

void AMovementCharacter::EnhancedInputMoveRight(const FInputActionValue& Value)
{
	float ValueRight = Value.Get<float>();
	EMovementMode CharacterMovementMode = GetCharacterMovement()->MovementMode;
	if ((Controller != nullptr) && (ValueRight != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		FVector Direction;
		if (GM && CharacterMovementMode == EMovementMode::MOVE_Flying)
		{
			//check if movable right or left on ledge if movement mode flying
			if (currentNinjaState)
			{
				currentNinjaState->HandleInput(this, InputTypeDirection::RIGHT_ITD, ValueRight);
			}

		}
		if (CharacterMovementMode != EMovementMode::MOVE_Flying)
		{
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, ValueRight);
		}

	}
	else if ((CharacterMovementMode == EMovementMode::MOVE_Flying) && (Controller != nullptr) && (ValueRight == 0.0f))
	{
		climbingState->LedgeHorizontalSpeed = 0.0f;
	}
}

void AMovementCharacter::EnhancedInputSwimUp(const FInputActionValue& Value)
{
	float SwimUpValue = Value.Get<float>();
	if ((Controller != nullptr) && (SwimUpValue != 0.0f) && (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Swimming))
	{
		if (currentNinjaState)
			currentNinjaState->HandleInput(this, InputTypeDirection::UP_ITD, SwimUpValue);
	}

}

void AMovementCharacter::ToggleFlashLight()
{	
	if (FlashLight)
	{
		if (FlashLight->IsVisible())
			FlashLight->SetVisibility(false);
		else
			FlashLight->SetVisibility(true);
	}
}

FVector AMovementCharacter::GetCrosshairProjectedWorldLocation()
{
	if (MovementCharacterPC)
	{
		MovementCharacterPC->DeprojectScreenPositionToWorld(ScreenMiddleCoordinates.X, ScreenMiddleCoordinates.Y, CrosshairProjectedWorldLocation, CrosshairProjectedWorldDirection);
		return CrosshairProjectedWorldLocation;
	}
	return FVector(0,0,0);
}

void AMovementCharacter::Pickup()
{
	UE_LOG(LogTemp, Warning, TEXT("Pressed Pickup"));
	//if (Weapon) {
	if (PotentialWeapon) {
		//TODO Pickup logic
		AddToInventory(PotentialWeapon->GetWeaponInventoryItem());
		FVector NewWeaponLocation = PotentialWeapon->GetActorLocation();
		TriggerPickup(PotentialWeapon->GetActorLocation());
		PotentialWeapon->WeaponPickedUp = true;
		if(armedState && !armedState->WeaponInHand)
			PotentialWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightShoulderSocket"));
		else if(armedState && armedState->WeaponInHand)
			PotentialWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandSocket"));
		if (Weapon)
		{
			Weapon->DetachRootComponentFromParent(false);
			Weapon->SetActorLocation(NewWeaponLocation);
			Weapon->WeaponPickedUp = false;
			TriggerPickup(Weapon->GetActorLocation());
			RemoveFromInventory(Weapon->GetWeaponInventoryItem());
			Weapon = PotentialWeapon;
			PotentialWeapon = nullptr;
		}
		else
		{
			Weapon = PotentialWeapon;
			PotentialWeapon = nullptr;
		}

		if (Weapon)
		{
			SecondsBetweenBullet = 1 / (Weapon->WeaponFireRate / 60.0f);
			UE_LOG(LogTemp, Warning, TEXT("SecondsBetweenBullet: %f"), SecondsBetweenBullet);
		}
		/*Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightShoulderSocket"));
		Weapon->WeaponPickedUp = true;
		TriggerPickup(Weapon->GetActorLocation());*/
	}
}

void AMovementCharacter::Aim()
{
	if (armedState && armedState->WeaponInHand && !armedState->IsAiming && GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
	{
		//CurrentLerpDuration = 0.0f;
		armedState->IsAiming = true;
		GetCharacterMovement()->MaxWalkSpeed = 100.f;
		TriggerAimStatus(armedState->IsAiming);//this is for notifying crosshair UI in level blueprint
		CameraBoom->TargetArmLength = 200.0f;
		CameraBoom->SocketOffset = FVector(0,85,75);
		/*if (Weapon)
		{
			//GM->DrawLineTrace(Weapon->WeaponSkeletalMeshComp->GetBoneLocation(FName(*FString(TEXT("b_gun_muzzleflash")))), Weapon->WeaponSkeletalMeshComp->GetBoneLocation(FName(*FString(TEXT("b_gun_muzzleflash")))) + (GetActorUpVector() * 500.0f), FColor::Cyan, false, 10.0f);
			//DrawDebugLine(GetWorld(), Weapon->WeaponSkeletalMeshComp->GetBoneLocation(FName(*FString(TEXT("b_gun_muzzleflash")))), Weapon->WeaponSkeletalMeshComp->GetBoneLocation(FName(*FString(TEXT("b_gun_muzzleflash")))) + (GetActorUpVector() * 500.0f),FColor::Cyan, false, 10.0f);
			FName GunMuzzleName = FName(FString("b_gun_muzzleflash"));
			FVector MuzzleLocation = Weapon->WeaponSkeletalMeshComp->GetBoneLocation(GunMuzzleName);
			DrawDebugLine(GetWorld(), MuzzleLocation, MuzzleLocation + (GetActorUpVector() * 500.0f),FColor::Cyan, false, 10.0f);
			Weapon->SpawnProjectile(MuzzleLocation + (GetActorForwardVector()*500.0f), GetActorRotation());
		}*/
		if (Weapon)
		{
			GetWorldTimerManager().SetTimer(TIMER_HANDLE_TIME_TO_AIM, this, &AMovementCharacter::TimeToAimElapsed, 1.0f, false, Weapon->TimeToAim);
		}
	}
}

void AMovementCharacter::StopAim()
{
	//if (armedState && armedState->WeaponInHand && armedState->IsAiming && GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
	if (armedState && armedState->WeaponInHand)
	{
		//CurrentLerpDuration = 0.0f;
		armedState->IsAiming = false;
		armedState->IsFireReady = false;
		GetWorldTimerManager().ClearTimer(TIMER_HANDLE_TIME_TO_AIM);
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
		TriggerAimStatus(armedState->IsAiming);
		CameraBoom->TargetArmLength = 300.0f;
		CameraBoom->SocketOffset = FVector(0, 65, 55);
	}
	FRotator FollowCameraCurrentRotation = FollowCamera->GetForwardVector().Rotation();
	FRotator CurrentPlayerRotation(0, FollowCameraCurrentRotation.Yaw, 0);
	SetActorRotation(CurrentPlayerRotation);
}

void AMovementCharacter::Fire()
{
	if (armedState && armedState->WeaponInHand && armedState->IsAiming && armedState->IsFireReady)
	{
		if (Weapon)
		{
			if (LastFireTimestampMillis != 0.0f)
			{
				double TempCurrentMillis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
					.count();
				UE_LOG(LogTemp, Warning, TEXT("<<<< computed diff %lf"), ((TempCurrentMillis - LastFireTimestampMillis) / 1000.0f));
				if ( ((TempCurrentMillis - LastFireTimestampMillis)/1000.0f) >= SecondsBetweenBullet)
				{
					LastFireTimestampMillis = TempCurrentMillis;
				}
				else
				{
					armedState->IsFiring = false;
					return;
				}
			}
			else
			{
				LastFireTimestampMillis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
					.count();
			}
			armedState->IsFiring = true;
			UE_LOG(LogTemp, Warning, TEXT("called %s"), ANSI_TO_TCHAR(__FUNCTION__));
			//TODO take decision of whether M4 being fired or As-Val? whichever weapon is being fired
			if (Weapon->WeaponInventoryItem->ItemDisplayName.ToString().Compare("M4-A1") == 0)//As-Val M4-A1
			{
				PlayerAudioComponent->SetSound(M4Sound);
			}
			else if (Weapon->WeaponInventoryItem->ItemDisplayName.ToString().Compare("As-Val") == 0)
			{
				PlayerAudioComponent->SetSound(ASValSound);
			}

			//GM->DrawLineTrace(Weapon->WeaponSkeletalMeshComp->GetBoneLocation(FName(*FString(TEXT("b_gun_muzzleflash")))), Weapon->WeaponSkeletalMeshComp->GetBoneLocation(FName(*FString(TEXT("b_gun_muzzleflash")))) + (GetActorUpVector() * 500.0f), FColor::Cyan, false, 10.0f);
			//DrawDebugLine(GetWorld(), Weapon->WeaponSkeletalMeshComp->GetBoneLocation(FName(*FString(TEXT("b_gun_muzzleflash")))), Weapon->WeaponSkeletalMeshComp->GetBoneLocation(FName(*FString(TEXT("b_gun_muzzleflash")))) + (GetActorUpVector() * 500.0f),FColor::Cyan, false, 10.0f);
			SpawnProjectile();

			//PlayerAudioComponent->Play();
		}
	}
}

void AMovementCharacter::StopFire()
{
	if (armedState)
	{
		armedState->IsFiring = false;
		GetWorldTimerManager().ClearTimer(TIMER_FIRE_RATE);
		if (PlayerAudioComponent->IsPlaying())
		{
			PlayerAudioComponent->Stop();
		}
	}
}

void AMovementCharacter::Use()
{
	if (armedState && !armedState->WeaponInHand && CurrentPickupItemInHand)
	{
		CurrentPickupItemInHand->Consume();
	}
}

void AMovementCharacter::SpawnProjectile()
{
	UE_LOG(LogTemp, Warning, TEXT("called %s"), ANSI_TO_TCHAR(__FUNCTION__));
	if (Weapon)
	{
		FName GunMuzzleName = FName(FString("b_gun_muzzleflash"));
		FVector MuzzleLocation = Weapon->WeaponSkeletalMeshComp->GetBoneLocation(GunMuzzleName);
		Weapon->SpawnProjectile(MuzzleLocation + (GetActorForwardVector() * 10.0f), GetActorForwardVector().Rotation());
	}
}

/*void AMovementCharacter::LerpAim(float DeltaSeconds)
{
	if (armedState && armedState->WeaponInHand && armedState->IsAiming && CurrentLerpDuration < 1.0f)
	{
		CameraBoom->TargetArmLength = FMath::Lerp(300.0f, 200.0f, CurrentLerpDuration / 1.0f);
		CameraBoom->SocketOffset = FMath::Lerp(FVector(0, 65, 55), FVector(0, 85, 75), CurrentLerpDuration / 1.0f);
		CurrentLerpDuration += DeltaSeconds;
	}
	else if (armedState && armedState->WeaponInHand && armedState->IsAiming && CurrentLerpDuration >= 1.0f)
	{
		CameraBoom->TargetArmLength = 200.0f;
		CameraBoom->SocketOffset = FVector(0, 85, 75);
	}
	else if (armedState && armedState->WeaponInHand && !armedState->IsAiming && CurrentLerpDuration < 1.0f)
	{
		CameraBoom->TargetArmLength = FMath::Lerp(200.0f, 300.0f, CurrentLerpDuration / 1.0f);
		CameraBoom->SocketOffset = FMath::Lerp(FVector(0, 85, 75), FVector(0, 65, 55), CurrentLerpDuration / 1.0f);
		CurrentLerpDuration += DeltaSeconds;
	}
	else if (armedState && armedState->WeaponInHand && !armedState->IsAiming && CurrentLerpDuration >= 1.0f)
	{
		CameraBoom->TargetArmLength = 300.0f;
		CameraBoom->SocketOffset = FVector(0, 65, 55);
	}
}*/
