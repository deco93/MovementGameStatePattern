// Copyright Epic Games, Inc. All Rights Reserved.

#include "MovementCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "SwimmingState.h"
#include "ClimbingState.h"
#include "NinjaState.h"
#include "MovementGameMode.h"


//////////////////////////////////////////////////////////////////////////
// AMovementCharacter

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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMovementCharacter::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMovementCharacter::OnEndOverlap);
	swimmingState = new SwimmingState();
	climbingState = new ClimbingState();
}

AMovementCharacter::~AMovementCharacter()
{
	if (swimmingState)
		delete swimmingState;

	if (climbingState)
		delete climbingState;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMovementCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMovementCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMovementCharacter::MoveRight);

	PlayerInputComponent->BindAxis("MoveUp", this, &AMovementCharacter::MoveUp);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMovementCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMovementCharacter::LookUpAtRate);

	
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
}

void AMovementCharacter::Tick(float DeltaSeconds)
{
	//UE_LOG(LogTemp, Warning, TEXT("inside char tick %d tick delta %f"), count++, DeltaSeconds);
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
		//UE_LOG(LogTemp, Warning, TEXT("current movement mode walking: %d swimming: %d"), uCharaceterMovement->MovementMode == EMovementMode::MOVE_Walking, uCharaceterMovement->MovementMode == EMovementMode::MOVE_Swimming);
	}
	if (GetCharacterMovement()->IsFalling())
	{
		if (GM)
		{
			FVector StartLoc = GetActorLocation() + GetActorForwardVector() * 35.0f;
			FHitResult OHit =GM->DrawLineTrace(GetActorLocation()+GetActorForwardVector()*35.0f, StartLoc + (GetActorForwardVector()* TraceDistance));
			FHitResult OHit2;
			if (OHit.bBlockingHit && OHit.Actor->ActorHasTag("Ledge"))
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

void AMovementCharacter::MoveForward(float Value)
{
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying)
		UE_LOG(LogTemp, Warning, TEXT("Value in move forward %f"), Value);

	if ((Controller != nullptr) && (Value != 0.0f) && (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Flying))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
	else if ((Controller != nullptr) && (Value != 0.0f) && (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying) && !climbingState->ClimbUp)
	{
	//else if ((Controller != nullptr) && (Value != 0.0f) && (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying) && !ClimbUp)
		climbingState->ClimbUp = true;
		if (currentNinjaState)
			currentNinjaState->HandleInput(this, InputTypeDirection::FORWARD_ITD, Value);
		//ClimbUp = true;
		GetWorld()->GetTimerManager().SetTimer(GM->TIMER_HANDLE,this, &AMovementCharacter::OnClimbComplete, 2.0f,false);
	}
}

void AMovementCharacter::MoveRight(float Value)
{
	EMovementMode CharacterMovementMode = GetCharacterMovement()->MovementMode;
	if ( (Controller != nullptr) && (Value != 0.0f) )
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
				currentNinjaState->HandleInput(this,InputTypeDirection::RIGHT_ITD,Value);
			}
			
		}
		if (CharacterMovementMode != EMovementMode::MOVE_Flying)
		{
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, Value);
		}
		
	}
	else if ((CharacterMovementMode == EMovementMode::MOVE_Flying) && (Controller != nullptr) && (Value == 0.0f))
	{
		climbingState->LedgeHorizontalSpeed = 0.0f;
	}
}

void AMovementCharacter::MoveUp(float Value)
{
	
	if ((Controller != nullptr) && (Value != 0.0f) && (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Swimming)) 
	{
		if(currentNinjaState)
			currentNinjaState->HandleInput(this, InputTypeDirection::UP_ITD, Value);
	}
	
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
