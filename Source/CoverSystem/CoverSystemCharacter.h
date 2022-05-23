// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"

#include "CoverSystemCharacter.generated.h"

class UCoverComponent;
class UBoxComponent;

UCLASS(config=Game)
class ACoverSystemCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Covers, meta = (AllowPrivateAccess = "true"))
	UCoverComponent* CoverComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
	Category = Covers, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Box;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CoverCapsule;

	UFUNCTION()
	void IsTakeCover();
	
public:

	void Tick(float DeltaSeconds) override;

	void BeginPlay() override;
	
	bool bAxisOn = true;
	FInputAxisBinding AxisForword;

	FInputAxisBinding AxisRight;

	FName AxisJump;
	FName AxisStopJump;

	
	//FInputAxisBinding AxisStopJump;

	void BindAndUnBind();
	void Test(float a);
	
	ACoverSystemCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Camera)
	float BaseLookUpRate;

	FORCEINLINE class UBoxComponent* GetBoxComponent() const { return Box; }

	// Right - 1, Left - (-1)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	int ValueForAimCover = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	bool InCover = 0;

	


protected:

	void MoveRightInCover(float Value);

	void MoveForwardInCover(float Value);

	void JumpOnCover();

	void СlimbOnCover();

	///////////////////////////////////////////////////////////////////////////////////
	
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

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

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION()
	void AimCover();
};

