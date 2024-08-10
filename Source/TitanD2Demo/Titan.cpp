
#include "Titan.h"
#include "Engine/LocalPlayer.h"
#include "Barricade.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

// Sets default values
ATitan::ATitan()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 450.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	isSliding = false;
	maxBoostMulti = 2.0;
	decayVal = .8f;
	slideMulti = 1.8f;
}

// Called when the game starts or when spawned
void ATitan::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATitan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATitan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATitan::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATitan::Look);

		//Sprinting
		//EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ATitan::Sprint);
		//EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ATitan::StopSprint);

		//Super
		EnhancedInputComponent->BindAction(SuperAction, ETriggerEvent::Triggered, this, &ATitan::CastSuper);

		//Sliding
		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Triggered, this, &ATitan::Slide);
		//EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Completed, this, &ATitan::StopSlide);

		//Barricade
		EnhancedInputComponent->BindAction(BarricadeAction, ETriggerEvent::Triggered, this, &ATitan::Barricade);

		//Grenade
		EnhancedInputComponent->BindAction(GrenadeAction, ETriggerEvent::Triggered, this, &ATitan::ThrowGrenade);

		//Shoulder Charge
		EnhancedInputComponent->BindAction(ShoulderChargeAction, ETriggerEvent::Triggered, this, &ATitan::ShoulderCharge);

		//Sword Swipe
		EnhancedInputComponent->BindAction(SwordSwipeAction, ETriggerEvent::Triggered, this, &ATitan::SwordSwipe);

		//Double Jump
		EnhancedInputComponent->BindAction(DoubleJumpAction, ETriggerEvent::Triggered, this, &ATitan::DoubleJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}
}

void ATitan::SpawnActor()
{
	FActorSpawnParameters spawnPar;
	spawnPar.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(actor, GetActorTransform(), spawnPar);
}

void ATitan::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}
void ATitan::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
void ATitan::Sprint(const FInputActionValue& Value)
{
	//GetCharacterMovement()->MaxWalkSpeed = 1000.f;
}
void ATitan::StopSprint(const FInputActionValue& Value)
{
	//GetCharacterMovement()->MaxWalkSpeed = 500.f;
}
void ATitan::Slide(const FInputActionValue& Value)
{
	if (isSliding == false && isMidAir == false && isRunning == true)
	{
		FVector velocity = GetCharacterMovement()->Velocity;
		FVector resVelocity = velocity * slideMulti;
		FVector* temp = new FVector;
		temp->Set(1, 1, 1);

		if (resVelocity.Size() <= 0)
		{
			GetCharacterMovement()->Launch(*temp * slideMulti);
		}
		else
		{
			GetCharacterMovement()->Launch(resVelocity);
		}
	}

}
void ATitan::CastSuper(const FInputActionValue& Value)
{
	//being handled in Blueprints
}
void ATitan::Barricade(const FInputActionValue& Value)
{
	//being handled in Blueprints
}
void ATitan::ThrowGrenade(const FInputActionValue& Value)
{
	//being handled in Blueprints
}
void ATitan::SwordSwipe(const FInputActionValue& Value)
{
	//being handled in Blueprints
}
void ATitan::ShoulderCharge(const FInputActionValue& Value)
{
	if (isMidAir == true && isRunning == true && isCharging == false)
	{
		FVector velocity = GetCharacterMovement()->Velocity;
		FVector resVelocity = velocity * chargeMulti;

		GetCharacterMovement()->Launch(resVelocity);
	}
}
void ATitan::DoubleJump(const FInputActionValue& Value)
{
	if (isMidAir == true)
	{
		//Get the rotation of pawn
		FVector velocity = GetCharacterMovement()->Velocity;
		GetCharacterMovement()->Launch(velocity * boostMulti);
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);

		DepleteBoost(velocity);
	}
}
void ATitan::DepleteBoost(FVector currentVel)
{
	boostMulti = FMath::Pow(boostMulti, decayVal);
}



