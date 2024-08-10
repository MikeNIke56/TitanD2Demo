#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <EnhancedInputComponent.h>
#include "Titan.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class TITAND2DEMO_API ATitan : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	ATitan();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void SpawnActor();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isJumping = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isMidAir = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float boostMulti;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxBoostMulti;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float decayVal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isSliding = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float slideMulti;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isRunning = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float chargeMulti;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isCharging = false;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SuperAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShoulderChargeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GrenadeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SlideAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BarricadeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwordSwipeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DoubleJumpAction;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);
	void Slide(const FInputActionValue& Value);
	void ThrowGrenade(const FInputActionValue& Value);
	void Barricade(const FInputActionValue& Value);
	void CastSuper(const FInputActionValue& Value);
	void ShoulderCharge(const FInputActionValue& Value);
	void SwordSwipe(const FInputActionValue& Value);
	void DoubleJump(const FInputActionValue& Value);

	UFUNCTION()
	void DepleteBoost(FVector currentVel);
};
