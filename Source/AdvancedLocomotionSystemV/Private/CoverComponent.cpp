


#include "CoverComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "D:\UE\UE_4.27\Engine\Source\Runtime\Engine\Classes\Engine\EngineTypes.h"
#include "D:\UE\UE_4.27\Engine\Source\Runtime\Engine\Public\CollisionQueryParams.h"
#include "D:\UE\UE_4.27\Engine\Source\Runtime\Engine\Classes\Engine\Engine.h"
#include "Math\Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine\Classes\Components\InputComponent.h"

// Sets default values for this component's properties
UCoverComponent::UCoverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistence = 100.0f;
	//CoverStateValue = CoverState::NoCover;
}


// Called when the game starts
void UCoverComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();

	if (Owner)
	{
		Character = Cast<ACharacter, AActor>(Owner);

		auto Move = Cast<UCharacterMovementComponent, UPawnMovementComponent>(Character->GetMovementComponent());
		if(Move)
			CharacterMovement = Move;

		Character->InputComponent->BindAction("AimOnCover", EInputEvent::IE_Pressed, this, &UCoverComponent::LookOnCover);
		
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,
			TEXT("Cover component is inclode"));
	}
	else
	{
		Character = nullptr;
		CharacterMovement = nullptr;
 
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,
			TEXT("Cover component isn't inclode"));
	}
	
	//Character->InputComponent->BindAction("Cover", IE_Pressed, this, &UCoverComponent::ControlIsTakeCover);

	/*
	TArray<FInputAxisBinding>* ArrayAxis = &Character->InputComponent->AxisBindings;

	for(int i = 0; i < ArrayAxis->Num(); ++i)
	{
		if((*ArrayAxis)[i].AxisName == "MoveForward")
		{
			AxisForword = (*ArrayAxis)[i];
		}

		if ((*ArrayAxis)[i].AxisName == "MoveRight")
		{
			AxisRight = (*ArrayAxis)[i];
		}
		
	}
	*/
}

FHitResult UCoverComponent::LetLineFront(float value)
{
	FHitResult Hit;

	FCollisionQueryParams TraceParams;

	FVector Start = Character->GetActorLocation();
	//Start += FVector(100.f,0.f, 0.f);
	FVector End = Start + (Character->GetActorForwardVector() * (TraceDistence / 2));

	Start.Z += value;
	End.Z += value;

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1);
	if (Hit.bBlockingHit)
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5),
			FColor::Red, false, 2.0f);

		//CoverStateValue = CoverState::FrontCover;
	}

	return Hit;
}

FHitResult UCoverComponent::LetLineBack(float value)
{
	FHitResult Hit;

	FCollisionQueryParams TraceParams;

	FVector Start = Character->GetActorLocation();
	FVector End = Start + ((Character->GetActorForwardVector() * -1)) * (TraceDistence / 2);

	Start.Z += value;
	End.Z += value;

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1);
	if (Hit.bBlockingHit)
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5),
			FColor::Red, false, 2.0f);

	}

	return Hit;
}

FHitResult UCoverComponent::RightVectorResultInCover(float height, float width)
{
	FHitResult HitRight;

	FCollisionQueryParams TraceParams;

	FVector Start = Character->GetActorLocation();

	Start.Z += height;
	Start.Y += width;

	auto Capsule = Character->GetCapsuleComponent();

	FVector ForwardVector = Capsule->GetForwardVector();

	FVector RightLocation = Start - Capsule->GetRightVector() * 50.f;

	FVector EndHeatVectorR = RightLocation + ForwardVector * (TraceDistence * -1);;

	//EndHeatVectorR.Z = RightLocation.Z = ForwardVector.Z = value;
	
	GetWorld()->LineTraceSingleByChannel(HitRight, RightLocation,
		EndHeatVectorR, ECC_Visibility, TraceParams);
	DrawDebugLine(GetWorld(), RightLocation,
		EndHeatVectorR, FColor::Red, false, 0.1);

	if (HitRight.bBlockingHit)
	{
		DrawDebugBox(GetWorld(), HitRight.ImpactPoint, FVector(5, 5, 5),
			FColor::Red, false, 2.0f);

		// todo вектор отражения
		auto Prikol = HitRight.ImpactPoint + (HitRight.Normal * 100);

		DrawDebugLine(GetWorld(), HitRight.ImpactPoint,
			Prikol, FColor::Yellow, false, 0.7);

		/*
		auto Prikol = EndHeatVectorR + HitRight.Normal;

		DrawDebugLine(GetWorld(), HitRight.ImpactPoint,
			Prikol, FColor::Yellow, false, 0.7);
		*/
	}

	return HitRight;
}

FHitResult UCoverComponent::LefttVectorResultInCover(float height, float width)
{
	FHitResult HitLeft;

	FCollisionQueryParams TraceParams;

	FVector Start = Character->GetActorLocation();

	Start.Z += height;
	
	Start += Character->GetCapsuleComponent()->GetRightVector() * width;


	auto Capsule = Character->GetCapsuleComponent();


	FVector LeftLocation = Start + Capsule->GetRightVector() * 50.f;


	FVector ForwardVector = Capsule->GetForwardVector();

	FVector EndHeatVector = LeftLocation + ForwardVector * (TraceDistence * -1); // (TraceDistence * -1);

	//EndHeatVector.Z = LeftLocation.Z = ForwardVector.Z = value;

	GetWorld()->LineTraceSingleByChannel(HitLeft, LeftLocation,
		EndHeatVector, ECC_Visibility, TraceParams);
	DrawDebugLine(GetWorld(), LeftLocation,
		EndHeatVector, FColor::Red, false, 0.1);

	/*	todo Сфера

		TArray<AActor*> ActorsToIgnore;
		//TArray<FHitResult> HitArray;
		FHitResult HitArray;

		UKismetSystemLibrary::SphereTraceSingle(GetWorld(),LeftLocation,EndHeatVector,10.f,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),false, ActorsToIgnore,
			EDrawDebugTrace::ForDuration, HitArray,true,FLinearColor::Gray,
			FLinearColor::Blue, 60.f);
	 */



	if (HitLeft.bBlockingHit)
	{
		DrawDebugBox(GetWorld(), HitLeft.ImpactPoint, FVector(5, 5, 5),
			FColor::Red, false, 2.0f);

		// todo вектор отражения
		auto Prikol = HitLeft.ImpactPoint + (HitLeft.Normal * 100);

		DrawDebugLine(GetWorld(), HitLeft.ImpactPoint,
			Prikol, FColor::Yellow, false, 0.7);
	}

	return HitLeft;
}

TArray<FHitResult> UCoverComponent::IsTakeCover()
{
	
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow,
		TEXT("IsTakeCover"));
	
	TArray<FHitResult> HitsLineTrace;
	
	FCollisionQueryParams TraceParams;

	UCapsuleComponent* Capsule = Character->GetCapsuleComponent();
	FVector ForwardVector = Capsule->GetForwardVector();
	FVector Location = Character->GetActorLocation();
	
	for (int i = 0; i <= 4; i++)
	{
		//Character->GetActorForwardVector();
		//GetOwner()->GetActorLocation();

		//FVector StartHeatVector1 = ;

		FHitResult Hit;

		FVector StartHeatVector = Location;

		switch (i)
		{
		case 0:
		{
			StartHeatVector.Z += 50.0f;
			break;
		}
		case 1:
		{
			StartHeatVector.Z += -50.0f;
			break;
		}
		case 2:
		{
			StartHeatVector -= Capsule->GetRightVector() * 25.f;
			break;
		}
		case 3:
		{
			StartHeatVector += Capsule->GetRightVector() * 25.f;
			break;
		}
		default:
		{
			break;
		}
		}

		FVector EndHeatVector = StartHeatVector + ForwardVector * TraceDistence;

		GetWorld()->LineTraceSingleByChannel(Hit, StartHeatVector, EndHeatVector,
			ECC_Visibility, TraceParams);

		DrawDebugLine(GetWorld(), StartHeatVector, EndHeatVector, FColor::Yellow,
			false, 1);

		if (Hit.bBlockingHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green,
				TEXT("Hit"));
			DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5),
				FColor::Red, false, 2.0f);

		}

		HitsLineTrace.Add(Hit);
		
	}
	
	return HitsLineTrace;
}

#pragma region	Control

// Called every frame
void UCoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LetLineFront();

	LetLineFront(60.f);


	LetLineBack();

	LetLineBack(60.f);

	FVector SpeedVector = FVector::ZeroVector;

	FVector VelocityCharacter = Character->GetVelocity();

	SpeedVector.X = VelocityCharacter.X;
	SpeedVector.Y = VelocityCharacter.Y;

	float SpeedCharacter = SpeedVector.Size();

	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, FString::Printf(TEXT("Sceed - %f"), SpeedCharacter));

	

	

	
#pragma region WASD

#pragma endregion 
	
	/*
	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow,
		FString::Printf(TEXT("Actor location: Yaw - %f, Pitch - %f, Roll- %f"), 
			Character->GetActorLocation().Y, Character->GetActorLocation().X, Character->GetActorLocation().Z));
	*/
	//FMath::Abs()
	
	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Blue,FString::Printf(TEXT("Vector Yaw - %f"), Character->GetActorForwardVector().Y));

	

	FVector VectorActorRotation= UKismetMathLibrary::GetRightVector(FRotator(0.0f, Character->GetActorRotation().Yaw, 0.0f));

	FVector VectorControllRotation = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, Character->GetControlRotation().Yaw, 0.0f));


	/*
	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Blue,
		FString::Printf(TEXT("Control Rotator: Yaw - %f, Pitch - %f, Roll- %f"),
			VectorControllRotation.Z, VectorControllRotation.Y, VectorControllRotation.X));

	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Blue,
		FString::Printf(TEXT("Actor Rotator: Yaw - %f, Pitch - %f, Roll- %f"),
			VectorActorRotation.Z, VectorActorRotation.Y, VectorActorRotation.X));


	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red,
		FString::Printf(TEXT("Control Rotator: Yaw - %f, Pitch - %f, Roll- %f"),
			Character->GetControlRotation().Yaw, Character->GetControlRotation().Pitch, Character->GetControlRotation().Roll));
	*/
	
	bool bLeftAndRight = UKismetMathLibrary::EqualEqual_VectorVector(VectorActorRotation, VectorControllRotation, 1.f);

	if (bLeftAndRight)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Orange,
			FString::Printf(TEXT("%i"), bLeftAndRight));

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Orange,
			FString::Printf(TEXT("%i"), bLeftAndRight));
	}

	
}


bool UCoverComponent::ControlIsTakeCover()
{
	
	TArray<FHitResult> CoverHits = IsTakeCover();
	
	bool bCoverToBe = true;

	for (int i = 0; i < CoverHits.Num(); ++i)
	{
		bCoverToBe = bCoverToBe && CoverHits[i].bBlockingHit;
	}

	if(bCoverToBe)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green,
			TEXT("IsCover"));

		//auto covernorm = CoverHits[0].Normal * -1;

		auto covernorm = CoverHits[0].Normal;
		Character->SetActorRotation((CoverHits[0].Normal * -1).Rotation());
		//Character->GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.f, 0.0f);

		CharacterMovement->MaxWalkSpeedCrouched = 200.f;
		CharacterMovement->MaxWalkSpeed = 200.f;

		//CoverTraceHeightOnMove = CoverTraceHeight;
		
		return bCoverToBe;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red,
			TEXT("IsNotCover"));
		//Character->GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

		CharacterMovement->MaxWalkSpeedCrouched = 650.f;
		CharacterMovement->MaxWalkSpeed = 650.f;
		
		return bCoverToBe;
	}

	/*
	if (!LetLineFront().bBlockingHit)
	{
		if (LetLineBack().bBlockingHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red,
				TEXT("OffCover"));
			Character->GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

			TArray<FInputAxisBinding>* ArrayAxis = &Character->InputComponent->AxisBindings;
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

			
			
			AxisForword = Character->InputComponent->BindAxis("MoveForward"&UCoverComponent::MoveForward);
			
			AxisRight = Character->InputComponent->BindAxis("MoveRight", this,
				&UCoverComponent::MoveRight);

			AxisRight = Character->InputComponent->BindAxis("MoveRight", this,
				&AxisRight);

		}
	}
	*/
}

bool UCoverComponent::MoveForwardInCover(float Value)
{
	if ((Character->GetController() != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Character->GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		Character->AddMovementInput(Direction, Value);
	}
	
	if (Value > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue,
			TEXT("forward"));
	}

	else if (Value < 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Black,
			TEXT("back"));
	}

	return true;

	//Character->GetController()->GetControlRotation().Yaw
}

bool UCoverComponent::MoveRightInCover(float Value, float TraceHeught)
{
	
	if (Character->Controller != nullptr)
	{
		FHitResult Hit;

		if (Value > 0.0f)
		{

			Hit = LefttVectorResultInCover(TraceHeught);

			if (Hit.bBlockingHit)
			{
				
				// find out which way is right
				const FRotator Rotation = (Hit.Normal * -1).Rotation();
				const FRotator YawRotation(0, Rotation.Yaw, 0);

				// get right vector 
				FVector Direction1 = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
				// add movement in that direction
				Character->AddMovementInput(Direction1, Value);

				Character->SetActorRotation(YawRotation);
				
			}
			
			else if(LefttVectorResultInCover(CoverTraceHeightCrouch - 20.f).bBlockingHit)
			{
				return false;
			}
			else
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Black,
					TEXT("forward"));
		
		}

		else if (Value < 0.0f)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString::Printf(TEXT("Sceed - %f"), Value));
			
			Hit = RightVectorResultInCover(TraceHeught);

			if (Hit.bBlockingHit)
			{
				// find out which way is right
				const FRotator Rotation = (Hit.Normal * -1).Rotation();
				const FRotator YawRotation(0, Rotation.Yaw, 0);

				// get right vector 
				FVector Direction1 = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
				// add movement in that direction
				Character->AddMovementInput(Direction1, Value);

				Character->SetActorRotation(YawRotation);
				
			}
			else if (RightVectorResultInCover(CoverTraceHeightCrouch - 20.f).bBlockingHit)
			{
				return false;
			}
			else
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Black,
					TEXT("back"));

		}
		
	}
	return true;
}

void UCoverComponent::SetDefaultValueCoverTraceHeightMoveState()
{
	CoverTraceHeightOnMove = CoverTraceHeightState;
}

void UCoverComponent::SetDefaultValueCoverTraceHeightMoveCrouch()
{
	CoverTraceHeightOnMove = CoverTraceHeightCrouch;
}

float UCoverComponent::ReturnDifference()
{
	/*
	float YawController = 0.f;
	float YawActor = 0.f;
	float YawrResult = 0.f;

	YawController = Character->GetController()->GetControlRotation().Yaw;

	YawActor =
		Character->GetActorRotation().Yaw > 0 ?
		Character->GetActorRotation().Yaw :
		Character->GetActorRotation().Yaw + 360;

	YawrResult = YawController - YawActor;
	
	if (YawrResult > 180)
	{
		if (YawController < 180)
			YawController -= 360;

		else if (YawActor < 180)
			YawActor -= 360;

		return YawrResult = YawController + YawActor;
	}
	else
	{
		return YawrResult = YawController - YawActor;
	}*/

	FHitResult Hit;

	FCollisionQueryParams TraceParams;

	FVector Start = Character->GetActorLocation();

	Start.Z = Start.Z + 20;

	FVector End = Start + Character->GetActorForwardVector() * (120);


	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
	
	auto C = Character->GetController()->GetControlRotation().Vector();
	auto H = Character->GetController()->GetControlRotation().Vector();

	//C.Z = 0;

	if (Hit.bBlockingHit)
	{
		H = Hit.Normal * -1;
	}

	float Dch = FVector::DotProduct(C, H);
	FVector Cch = FVector::CrossProduct(C, H);

	float Mc = C.Size();
	float Mh = H.Size();

	float angle = FMath::Acos(Dch / (Mc * Mh)) * Cch.Z * -1 * 100;

	return angle;
}


FVector UCoverComponent::GetFrontLineTraceNormalVector()
{
	return CoverNormal = LetLineFront(60.f).Normal;
}

FRotator UCoverComponent::GetFrontLineTraceNormalRotation()
{
	return  LetLineFront(60.f).Normal.Rotation();
}

bool UCoverComponent::LineTrace(float height, float width)
{
	FHitResult HitLeft;

	FCollisionQueryParams TraceParams;

	FVector Start = Character->GetActorLocation();

	Start.Z += height;


	auto Capsule = Character->GetCapsuleComponent();


	FVector LeftLocation = Start + Capsule->GetRightVector() * width;


	FVector ForwardVector = Capsule->GetForwardVector();

	FVector EndHeatVector = LeftLocation + ForwardVector * (TraceDistence * -1);

	GetWorld()->LineTraceSingleByChannel(HitLeft, LeftLocation,
		EndHeatVector, ECC_Visibility, TraceParams);
	DrawDebugLine(GetWorld(), LeftLocation,
		EndHeatVector, FColor::Blue, false, 0.1);

	return HitLeft.bBlockingHit;
}

int UCoverComponent::MoveInCoverForward_New(float Value, float TraceHeught)
{
	// 0 - Nope
	// 1 - Crounch
	// 2 - NoCoverHit
	// 3 - CoverHit
	
	FHitResult Hit;

	/*
	if(
		LineTrace(height_newmoveR, width_newmoveR) &&
		LineTrace(height_newmoveL, width_newmoveL))
	{
		CharacterMovement->MaxWalkSpeedCrouched = 300.f;
		CharacterMovement->MaxWalkSpeed = 300.f;
	}
	else
	{
		CharacterMovement->MaxWalkSpeedCrouched = 100.f;
		CharacterMovement->MaxWalkSpeed = 100.f;
	}*/

	FVector VectorActorRotation = UKismetMathLibrary::GetRightVector(FRotator(0.0f, Character->GetActorRotation().Yaw, 0.0f));

	FVector VectorControllRotation = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, Character->GetControlRotation().Yaw, 0.0f));

	bool bLeftAndRight = UKismetMathLibrary::EqualEqual_VectorVector(VectorActorRotation, VectorControllRotation, 1.f);

	
	if((Character->GetController() != nullptr) && (Value != 0.0f))
	{
		
		if((Value < 0 && !bLeftAndRight) || (Value > 0 && bLeftAndRight))
		{
			Hit = LefttVectorResultInCover(CoverTraceHeightOnMove);
			if(LineTrace(height_newmoveR, width_newmoveR))
			{
				CharacterMovement->MaxWalkSpeedCrouched = 300.f;
				CharacterMovement->MaxWalkSpeed = 300.f;
			}
			else
			{
				CharacterMovement->MaxWalkSpeedCrouched = 100.f;
				CharacterMovement->MaxWalkSpeed = 100.f;
			}

			ValueForAimCover = 1;
		}

		else if ((Value < 0 && bLeftAndRight) || (Value > 0 && !bLeftAndRight))
		{
			Hit = RightVectorResultInCover(CoverTraceHeightOnMove);
			if (LineTrace(height_newmoveL, width_newmoveL))
			{
				CharacterMovement->MaxWalkSpeedCrouched = 300.f;
				CharacterMovement->MaxWalkSpeed = 300.f;
			}
			else
			{
				CharacterMovement->MaxWalkSpeedCrouched = 100.f;
				CharacterMovement->MaxWalkSpeed = 100.f;
			}

			ValueForAimCover = -1;
		}

		if(Hit.bBlockingHit)
		{
			// find out which way is right
			const FRotator Rotation = Character->GetController()->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			// add movement in that direction
			Character->AddMovementInput(Direction, Value);

			return 3;
		}
		else
		{
			FHitResult NewHit;

			if (Value < 0 && !bLeftAndRight)
			{
				NewHit = LefttVectorResultInCover(TraceHeught);
			}

			else if (Value < 0 && bLeftAndRight)
			{
				NewHit = RightVectorResultInCover(TraceHeught);
			}

			else if (Value > 0 && !bLeftAndRight)
			{
				NewHit = RightVectorResultInCover(TraceHeught);
			}

			else if (Value > 0 && bLeftAndRight)
			{
				NewHit = LefttVectorResultInCover(TraceHeught);
			}

			if (NewHit.bBlockingHit)
			{
				CoverTraceHeightOnMove = CoverTraceHeightCrouch;
				CoverStateEnum = CoverState::Crouch;

				return 1;
			}
			else
			{
				return 2;
			}
			
		}
		
		
	}
	
	return 0;
}

int UCoverComponent::MoveInCoverRight_New(float Value, float TraceHeught)
{
	// 0 - Nope
	// 1 - Crounch
	// 2 - NoCoverHit
	// 3 = CoverHit
	
	FHitResult Hit;

	if ((Character->GetController() != nullptr) && (Value != 0.0f))
	{
		
		if (Value > 0)
		{
			Hit = RightVectorResultInCover(CoverTraceHeightOnMove);
			
			if (Hit.bBlockingHit)
			{

				if (LineTrace(height_newmoveL, width_newmoveL))
				{
					CharacterMovement->MaxWalkSpeedCrouched = 300.f;
					CharacterMovement->MaxWalkSpeed = 300.f;
				}
				else
				{
					CharacterMovement->MaxWalkSpeedCrouched = 100.f;
					CharacterMovement->MaxWalkSpeed = 100.f;
				}
			}

			ValueForAimCover = -1;
		}

		else if (Value < 0)
		{
			Hit = LefttVectorResultInCover(CoverTraceHeightOnMove);

			if (Hit.bBlockingHit)
			{
				
				if (LineTrace(height_newmoveR, width_newmoveR))
				{
					CharacterMovement->MaxWalkSpeedCrouched = 300.f;
					CharacterMovement->MaxWalkSpeed = 300.f;
				}
				else
				{
					CharacterMovement->MaxWalkSpeedCrouched = 100.f;
					CharacterMovement->MaxWalkSpeed = 100.f;
				}
				
				
			}

			ValueForAimCover = 1;
		}

		if(Hit.bBlockingHit)
		{
			// find out which way is right
			const FRotator Rotation = (CoverNormal * -1).Rotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			Character->AddMovementInput(Direction, Value);
			return 3;
		}
		else
		{
			FHitResult NewHit;

			if (Value > 0)
			{
				NewHit = RightVectorResultInCover(TraceHeught);
			}
			else if (Value < 0)
			{
				NewHit = LefttVectorResultInCover(TraceHeught);
			}

			if (NewHit.bBlockingHit)
			{
				CoverTraceHeightOnMove = CoverTraceHeightCrouch;
				CoverStateEnum = CoverState::Crouch;

				return 1;
			}
			else
			{
				return 2;
			}

		}
	}

	return 0;
}

// true - state : false - crouch
void UCoverComponent::SetStateEnum(bool key)
{
	if (key)
		CoverStateEnum = CoverState::State;
	else
		CoverStateEnum = CoverState::Crouch;


	
	if (CoverStateEnum == CoverState::State)
	{
		CoverTraceHeightOnMove = CoverTraceHeightState;
	}

	else if (CoverStateEnum == CoverState::Crouch)
	{
		CoverTraceHeightOnMove = CoverTraceHeightCrouch;
	}
}

int UCoverComponent::AimCover(bool stay)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange,
		FString::Printf(TEXT("Aim - %i"), ValueForAimCover));

	/*
		1 - слева
		2 - справа
		3 - верх
		0 - нет укрытия
	 */
	
	switch (ValueForAimCover)
	{
		case -1:
		{
			FHitResult Hit = RightVectorResultInCover(40.f);

			if (!Hit.bBlockingHit)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green,
					FString::Printf(TEXT("Слева можно выглянуть")));
				return 1;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red,
					FString::Printf(TEXT("Слева нельзя выглянуть")));
				break;
			}
			
		}

		case 1:
		{
			FHitResult Hit = LefttVectorResultInCover(40.f);

			if (!Hit.bBlockingHit)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green,
					FString::Printf(TEXT("Справа есть укрытие")));
				return 2;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red,
					FString::Printf(TEXT("Справа нет укрытия")));
				break;
			}
		}

		
		default:
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange,
				FString::Printf(TEXT("Суета")));
			break;
		}
		
	}

	if (stay)
	{
		if (!LetLineBack(80.f).bBlockingHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue,
				FString::Printf(TEXT("Сверху можно выглянуть!")));
			return 3;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red,
				FString::Printf(TEXT("Сверху НЕЛЬЗЯ выглянуть!")));
		}
	}
	
	return 0;
}

bool UCoverComponent::CoverAfterStanding()
{
	FHitResult Hit = LetLineBack(60.f);

	return Hit.bBlockingHit;
	
}


// bindaction
void UCoverComponent::LookOnCover()
{
	look.Broadcast();
}

#pragma endregion 

