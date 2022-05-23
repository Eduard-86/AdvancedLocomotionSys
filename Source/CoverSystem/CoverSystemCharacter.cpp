// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoverSystemCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "CoverComponent/CoverComponent.h"
#include "DrawDebugHelpers.h"
#include <ThirdParty/VHACD/inc/btScalar.h>

//////////////////////////////////////////////////////////////////////////
// ACoverSystemCharacter

/*
void ACoverSystemCharacter::BeginPlay()
{
	
}*/

ACoverSystemCharacter::ACoverSystemCharacter()
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

	CoverComponent = CreateDefaultSubobject<UCoverComponent>(TEXT("CoverComponent"));
	
	/*
	CoverCapsule = CreateDefaultSubobject<UCapsuleComponent>("CoverCapsule");
	CoverCapsule->InitCapsuleSize(60.0f, 88.0f);

	CoverCapsule->CanCharacterStepUpOn = ECB_No;
	CoverCapsule->SetShouldUpdatePhysicsVolume(true);
	CoverCapsule->SetCanEverAffectNavigation(false);
	CoverCapsule->bDynamicObstacle = true;

	CoverCapsule->SetupAttachment(RootComponent);
	*/
	
	Box = CreateDefaultSubobject<UBoxComponent>("Box");

	Box->SetupAttachment(GetRootComponent());

}


void ACoverSystemCharacter::OnResetVR()
{
	// If CoverSystem is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in CoverSystem.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ACoverSystemCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ACoverSystemCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ACoverSystemCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACoverSystemCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACoverSystemCharacter::MoveForward(float Value)
{
	switch (CoverComponent->CoverStateValue)
	{
		case CoverState::NoCover:
		{
			if ((Controller != nullptr) && (Value != 0.0f))
			{
				// find out which way is right
				const FRotator Rotation = Controller->GetControlRotation();
				const FRotator YawRotation(0, Rotation.Yaw, 0);

				// get right vector 
				FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
				// add movement in that direction
				AddMovementInput(Direction, Value);
			}
			
			break;
		}

		case CoverState::FrontCover:
		{
			if ((Controller != nullptr) && (Value != 0.0f))
			{
				// find out which way is right
				const FRotator Rotation = Controller->GetControlRotation();
				const FRotator YawRotation(0, Rotation.Yaw, 0);

				// get right vector 
				FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
				// add movement in that direction
				AddMovementInput(Direction, Value);
			}

			break;
		}
		
		case CoverState::InCover:
		{
				
				// find out which way is right
				const FVector RightVector = GetCapsuleComponent()->GetRightVector();
				const FRotator YawRotation(0, RightVector.Y, 0);

				// get right vector 
				FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
				// add movement in that direction
				AddMovementInput(Direction, Value);
		}
			
			
			break;	
	}

	/*
	if(CoverComponent->CoverStateValue == CoverState::BehindCover)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow,
			TEXT("Axis всегда тикает!"));
		
	}*/
	
	
}

void ACoverSystemCharacter::MoveRight(float Value)
{
	/*
	if(Value > 0)
	{
		CoverComponent->LefttVectorResultInCover();
	}

	else if (Value < 0)
	{
		CoverComponent->RightVectorResultInCover();
	}*/
			
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		FVector Direction1 = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction1, Value);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void ACoverSystemCharacter::Test(float a)
{
	a = a = a = a = a;
}

void ACoverSystemCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACoverSystemCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump",
		IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump",
		IE_Released, this, &ACharacter::StopJumping);



	AxisForword = PlayerInputComponent->BindAxis("MoveForward", this, &ACoverSystemCharacter::MoveForward);

	//AxisForvord = Axis;

	AxisRight = PlayerInputComponent->BindAxis("MoveRight", this, &ACoverSystemCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACoverSystemCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACoverSystemCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACoverSystemCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACoverSystemCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACoverSystemCharacter::OnResetVR);

	//***********************************

	PlayerInputComponent->BindAction("OnAxis", IE_Pressed, this, &ACoverSystemCharacter::BindAndUnBind);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACoverSystemCharacter::Test);

	PlayerInputComponent->BindAction("Cover", IE_Pressed, this, &ACoverSystemCharacter::IsTakeCover);

	PlayerInputComponent->BindAction("AimCover", IE_Pressed, this, &ACoverSystemCharacter::AimCover);


}

void ACoverSystemCharacter::AimCover()
{
	/*
	 *	Запустим проверку, в зависимости от позиции персонажа
	 *	
	 * 
	 */

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange,
		FString::Printf(TEXT("Aim - %i"), ValueForAimCover));

	switch(ValueForAimCover)
	{
		case -1:
			{
				FHitResult Hit = CoverComponent->RightVectorResultInCover(40.f);

				if(!Hit.bBlockingHit)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green,
						FString::Printf(TEXT("На лево прикол есть !")));
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red,
						FString::Printf(TEXT("На лево прикол не найден !")));
				}
				
				break;
			}

		case 1:
			{
				FHitResult Hit = CoverComponent->LefttVectorResultInCover(40.f);

				if (!Hit.bBlockingHit)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green,
						FString::Printf(TEXT("На право прикол есть !")));
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red,
						FString::Printf(TEXT("На право прикол не найден !")));
				}
				break;
			}

		default :
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange,
				FString::Printf(TEXT("Суета")));
			break;
		}
	}

}


void ACoverSystemCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CoverComponent->LetLineFront();

	CoverComponent->LetLineFront(60.f);


	CoverComponent->LetLineBack();

	CoverComponent->LetLineBack(60.f);

}

void ACoverSystemCharacter::IsTakeCover()
{
	// TODO заглушка заменить на AB
	if(!CoverComponent->LetLineFront().bBlockingHit)
	{
		if(CoverComponent->LetLineBack().bBlockingHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red,
				TEXT("OffCover"));
			GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
			
			TArray<FInputAxisBinding>* ArrayAxis = &InputComponent->AxisBindings;
			for (int i = ArrayAxis->Num() - 1; i > -1; i--)
			{
				if ((*ArrayAxis)[i].AxisDelegate.GetDelegate().GetHandle() ==
					AxisForword.AxisDelegate.GetDelegate().GetHandle())
				{
					ArrayAxis->RemoveAt(i);
					continue;
				}

				if ((*ArrayAxis)[i].AxisDelegate.GetDelegate().GetHandle() ==
					AxisRight.AxisDelegate.GetDelegate().GetHandle())
				{
					ArrayAxis->RemoveAt(i);
				}
			}

			AxisForword = InputComponent->BindAxis("MoveForward", this,
				&ACoverSystemCharacter::MoveForward);
			AxisRight = InputComponent->BindAxis("MoveRight", this,
				&ACoverSystemCharacter::MoveRight);
			
		}
	}
	
	
	
	auto CoverHits = CoverComponent->IsTakeCover();

	bool bCoverToBe = true;
	
	for(int i = 0; i < CoverHits.Num(); ++i)
	{
		bCoverToBe = bCoverToBe && CoverHits[i].bBlockingHit;
	}

	InCover = bCoverToBe;
	
	if (bCoverToBe)
	{
		SetActorRotation((CoverHits[0].Normal * -1).Rotation());
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);

		
		TArray<FInputAxisBinding>* ArrayAxis = &InputComponent->AxisBindings;
		for (int i = ArrayAxis->Num() - 1; i > -1; i--)
		{
			if ((*ArrayAxis)[i].AxisDelegate.GetDelegate().GetHandle() ==
				AxisForword.AxisDelegate.GetDelegate().GetHandle())
			{
				ArrayAxis->RemoveAt(i);
				continue;
			}
			
			if ((*ArrayAxis)[i].AxisDelegate.GetDelegate().GetHandle() ==
				AxisRight.AxisDelegate.GetDelegate().GetHandle())
			{
				ArrayAxis->RemoveAt(i);
			}
		}

		AxisForword = InputComponent->BindAxis("MoveForward", this, 
			&ACoverSystemCharacter::MoveForwardInCover);
		AxisRight = InputComponent->BindAxis("MoveRight", this, 
			&ACoverSystemCharacter::MoveRightInCover);

		//InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

		InputComponent->RemoveActionBinding("Jump", IE_Pressed);
		InputComponent->RemoveActionBinding("Jump", IE_Released);
		
		InputComponent->BindAction("Jump",
			IE_Pressed, this, &ACoverSystemCharacter::СlimbOnCover);

	}
}

void ACoverSystemCharacter::MoveForwardInCover(float Value)
{
	
	if(Value > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue,
			TEXT("forward"));
	}

	else if (Value < 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Black,
			TEXT("back"));
	}
	
	return;
	
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ACoverSystemCharacter::MoveRightInCover(float Value)
{
	if (Controller != nullptr)
	{
		FHitResult Hit;
		
		if(Value > 0.0f)
		{
			ValueForAimCover = Value;
			
			Hit = CoverComponent->LefttVectorResultInCover(40.f);

			//GetWorld()->LineTraceMultiByChannel()

			
			
			//UKismetSystemLibrary::SphereTraceSingleForObjects();

			

			if(!Hit.bBlockingHit)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Black,
					TEXT("forward"));

				
				
				return;
			}
			// find out which way is right
			const FRotator Rotation = (Hit.Normal * -1).Rotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			FVector Direction1 = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction1, Value);

			SetActorRotation(YawRotation);
		}

		else if (Value < 0.0f)
		{
			ValueForAimCover = Value;
			
			Hit = CoverComponent->RightVectorResultInCover();

			if (!Hit.bBlockingHit)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Black,
					TEXT("back"));
				return;
			}
			
			// find out which way is right
			const FRotator Rotation = (Hit.Normal * -1).Rotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			FVector Direction1 = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction1, Value);

			SetActorRotation(YawRotation);
		}
	}
}

void ACoverSystemCharacter::BindAndUnBind()
{
	//bUseControllerRotationYaw = true;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
	
	if(bAxisOn)
	{
		TArray<FInputAxisBinding>* ArrayAxis = &InputComponent->AxisBindings;
		for (int i = ArrayAxis->Num() - 1; i > -1; i--)
		{
			auto aa = (*ArrayAxis)[i].AxisDelegate.GetDynamicDelegate().GetFunctionName();
			auto aa1 = AxisForword.AxisDelegate.GetDynamicDelegate().GetFunctionName();

			if ((*ArrayAxis)[i].AxisDelegate.GetDelegate().GetHandle() ==
				AxisForword.AxisDelegate.GetDelegate().GetHandle())
			{
				ArrayAxis->RemoveAt(i);
			}

			/*
			if((*ArrayAxis)[i].AxisDelegate.GetDynamicDelegate().GetFunctionName() ==
				AxisForvord.AxisDelegate.GetDynamicDelegate().GetFunctionName() &&
				(*ArrayAxis)[i].AxisName == AxisForvord.AxisName)
			{
				ArrayAxis->RemoveAt(i);
			}
			*/
		}
		bAxisOn = false;
	}
	
	else
	{
		AxisForword = InputComponent->BindAxis("MoveForward", this, &ACoverSystemCharacter::MoveForward);

		bAxisOn = true;
	}
	
	//InputComponent->RemoveActionBinding();
	
	//ArrayAxis->Last()
	//ArrayAxis->RemoveAt(1);
	//InputComponent->AxisBindings.RemoveAt();
}

void ACoverSystemCharacter::СlimbOnCover()
{
	auto Hit = CoverComponent->LetLineBack(140.f);
	if(!Hit.bBlockingHit)
	{
		TArray<AActor*> ActorsToIgnore;
		//TArray<FHitResult> HitArray;
		FHitResult HitArray;
		
		Hit.Location;

		UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Hit.Location, Hit.Location * 10, 10.f,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore,
			EDrawDebugTrace::ForDuration, HitArray, true, FLinearColor::Gray,
			FLinearColor::Blue, 60.f);
	}
	
}