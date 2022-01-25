


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

FHitResult UCoverComponent::RightVectorResultInCover(float value)
{
	FHitResult HitRight;

	FCollisionQueryParams TraceParams;

	FVector Start = Character->GetActorLocation();

	Start.Z += value;

	auto Capsule = Character->GetCapsuleComponent();

	FVector ForwardVector = Capsule->GetForwardVector();

	FVector RightLocation = Start - Capsule->GetRightVector() * 50.f;

	FVector EndHeatVectorR = RightLocation + ForwardVector * TraceDistence;

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

FHitResult UCoverComponent::LefttVectorResultInCover(float value)
{
	FHitResult HitLeft;

	FCollisionQueryParams TraceParams;

	FVector Start = Character->GetActorLocation();

	Start.Z += value;


	auto Capsule = Character->GetCapsuleComponent();


	FVector LeftLocation = Start + Capsule->GetRightVector() * 50.f;


	FVector ForwardVector = Capsule->GetForwardVector();

	FVector EndHeatVector = LeftLocation + ForwardVector * TraceDistence;

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

	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, FString::Printf(TEXT("Sceed - %f"), SpeedCharacter));

	float YawController = 0.f;
	float YawActor = 0.f;
	float YawrResult = 0.f;
	
	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, 
		FString::Printf(TEXT("Yaw - %f"), YawController = Character->GetController()->GetControlRotation().Yaw));

	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow,
		FString::Printf(TEXT("Actor Yaw - %f"), YawActor = Character->GetActorRotation().Yaw > 0 ? Character->GetActorRotation().Yaw : Character->GetActorRotation().Yaw + 360));

	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red,
		FString::Printf(TEXT("Result - %f"), YawrResult = YawController - YawActor));
	
	/*
	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow,
		FString::Printf(TEXT("Actor location: Yaw - %f, Pitch - %f, Roll- %f"), 
			Character->GetActorLocation().Y, Character->GetActorLocation().X, Character->GetActorLocation().Z));
	*/
	//FMath::Abs()
	
	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Blue,FString::Printf(TEXT("Vector Yaw - %f"), Character->GetActorForwardVector().Y));
	
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

		CharacterMovement->MaxWalkSpeedCrouched = 300.f;
		CharacterMovement->MaxWalkSpeed = 300.f;

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
	if ((Character->Controller != nullptr) && (Value != 0.0f))
	{
		/*todo Углы камеры относительно актора
		float ActorYaw = Character->GetActorRotation().Yaw > 0 ? Character->GetActorRotation().Yaw :
			Character->GetActorRotation().Yaw + 360;

		float ControllerYaw = Character->GetController()->GetControlRotation().Yaw;
		*/
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

#pragma endregion 

