// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverComponent/CoverComponent.h"
#include "CoverSystem/CoverSystemCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"

#include "D:\Epic Game\UE_4.26\Engine\Source\Runtime\Engine\Classes\GameFramework\CharacterMovementComponent.h"
#include "D:\Epic Game\UE_4.26\Engine\Source\Runtime\Engine\Classes\GameFramework\PawnMovementComponent.h"
#include "D:\Epic Game\UE_4.26\Engine\Source\Runtime\Engine\Classes\GameFramework\SpringArmComponent.h"

#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UCoverComponent::UCoverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	TraceDistence = 100.0f;
	CoverStateValue = NoCover;
	
}

// Called when the game starts
void UCoverComponent::BeginPlay()
{
	Super::BeginPlay();

	//todo Запретить вешать не на Character 

	AActor* Owner = GetOwner();

	if (Owner)
	{
		Character = Cast<ACoverSystemCharacter, AActor>(Owner);
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red,
			TEXT("Прикол"));

		UBoxComponent* Box = Character->GetBoxComponent();

		// Расчёт прилипания 
		Box->OnComponentBeginOverlap.AddDynamic(this, &UCoverComponent::CoverCheck);
	}

	//Character->InputComponent->BindAction(FName("Cover"), IE_Pressed, this, &UCoverComponent::CoverCheck);

	//Character->InputComponent->BindAction("Cover", IE_Pressed, this, &UCoverComponent::OnTakeCover);
	//Character->InputComponent->RemoveActionBinding()
}

void UCoverComponent::CoverCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green,
		TEXT("Бокс!"));

	Character->GetActorForwardVector();
	
}

inline void UCoverComponent::OnTakeCover()
{
	/*
	// find out which way is forward
	const FRotator Rotation = Character->Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	Character->Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	*/

	// Массив axis
	Character->InputComponent->AxisBindings[0];
	
	if(CoverStateValue == CoverState::FrontCover)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow,
			TEXT("OnTakeCover"));
		
		TArray<FHitResult> HitsLineTrace;
		

		for(int i = 0; i <= 4; i++)
		{
			//Character->GetActorForwardVector();
			//GetOwner()->GetActorLocation();

			//FVector StartHeatVector1 = ;

			FHitResult Hit;
			FCollisionQueryParams TraceParams;

			auto Capsule = Character->GetCapsuleComponent();
			FVector ForwardVector = Capsule->GetForwardVector();
			FVector Location = Character->GetActorLocation();
			FVector RightVector = Capsule->GetRightVector();

			FVector StartHeatVector = Location;
			
			switch (i)
			{
				case 0:
				{
					StartHeatVector.Z += 50.0f;
					//StartHeatVector1.Z += 50.0f;
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

			GetWorld()->LineTraceSingleByChannel(Hit, StartHeatVector, EndHeatVector, ECC_Visibility, TraceParams);

			DrawDebugLine(GetWorld(), StartHeatVector, EndHeatVector, FColor::Yellow, false, 1);
			
			if (Hit.bBlockingHit)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green,
					TEXT("Hit"));
				DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5),
					FColor::Red, false, 2.0f);

				//Character->GetCharacterMovement()->

				//Character->GetCharacterMovement()->bOrientRotationToMovement = false;

				//Character->bUseControllerRotationYaw = true;

			}

			HitsLineTrace.Add(Hit);
			
		}

		bool bCoverToBe = true;
		
		for(int i = 0; i < HitsLineTrace.Num(); i++)
		{
			//bCoverToBe = HitsLineTrace[i]->bBlockingHit;

			bCoverToBe = bCoverToBe && (bool)HitsLineTrace[i].bBlockingHit;

		}

		if(bCoverToBe)
		{

			CoverStateValue = InCover;

			
			Character->SetActorRotation(HitsLineTrace[0].Normal.Rotation());
			Character->GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
			
		}
		/*
		// Перемещение ближе к стене

		FVector ForwardVector = Character->GetCapsuleComponent()->GetForwardVector();
		if(bCoverToBe)
		{
			Character->AddActorWorldOffset(HitsLineTrace[0].Location);
			//Character->AddMovementInput(ForwardVector, 20000.f);
			//Character->GetMovementComponent()->Move
			
		}*/
		
	}
}

/*
FHitResult UCoverComponent::LetLineFront()
{
	FHitResult Hit;
	
	TArray<FHitResult> HitsLineTrace;
	

	FCollisionQueryParams TraceParams;
	
	FVector Start = Character->GetActorLocation();
	//Start += FVector(100.f,0.f, 0.f);
	FVector End = Start + (Character->GetActorForwardVector() * (TraceDistence / 2));


	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1);
	if (Hit.bBlockingHit)
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5),
			FColor::Red, false, 2.0f);

		CoverStateValue = FrontCover;
	}

	Start.Z += 60.f;
	End.Z += 60.f;
	
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1);

	if (Hit.bBlockingHit)
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5),
			FColor::Red, false, 2.0f);

		CoverStateValue = FrontCover;
	}

	
	
	return Hit;
}

FHitResult UCoverComponent::LetLineBack()
{
	FHitResult Hit;

	FCollisionQueryParams TraceParams;
	
	FVector Start = Character->GetActorLocation();
	FVector End = Start + ((Character->GetActorForwardVector() * -1)) * (TraceDistence / 2);


	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1);
	if (Hit.bBlockingHit)
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5),
			FColor::Red, false, 2.0f);

	}

	Start.Z += 60.f;
	End.Z += 60.f;

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1);

	if (Hit.bBlockingHit)
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5),
			FColor::Red, false, 2.0f);

		CoverStateValue = FrontCover;
	}

	return Hit;

}
*/

// heat 60.f
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

		CoverStateValue = FrontCover;
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

/*
FHitResult UCoverComponent::LetThighLineFront()
{
	FHitResult Hit;

	FCollisionQueryParams TraceParams;

	FVector Start = Character->GetActorLocation();
	FVector End = Start + (Character->GetActorForwardVector() * (TraceDistence / 2));


	Start.Z += 60.f;
	End.Z += 60.f;

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1);

	if (Hit.bBlockingHit)
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5),
			FColor::Red, false, 2.0f);

		//CoverStateValue = FrontCover;
	}

	return Hit;
}

FHitResult UCoverComponent::LetThighLineBack()
{
	FHitResult Hit;

	FCollisionQueryParams TraceParams;

	FVector Start = Character->GetActorLocation();
	FVector End = Start + ((Character->GetActorForwardVector() * -1)) * (TraceDistence / 2);

	Start.Z += 60.f;
	End.Z += 60.f;

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1);

	if (Hit.bBlockingHit)
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5),
			FColor::Red, false, 2.0f);

		//CoverStateValue = FrontCover;
	}

	return Hit;
}
*/

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
	
	
	
	/*
	// Перемещение ближе к стене

	FVector ForwardVector = Character->GetCapsuleComponent()->GetForwardVector();
	if(bCoverToBe)
	{
		Character->AddActorWorldOffset(HitsLineTrace[0].Location);
		//Character->AddMovementInput(ForwardVector, 20000.f);
		//Character->GetMovementComponent()->Move

	}*/

	
}

// Called every frame
void UCoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult Hit;

	//Character->GetController()->GetPlayerViewPoint(Loc, Rot);

	switch (CoverStateValue)
	{
		case NoCover:
		{
			FVector Start = GetOwner()->GetActorLocation();
			//Start += FVector(100.f,0.f, 0.f);
			FVector End = Start + (GetOwner()->GetActorForwardVector() * (TraceDistence / 2));

			FCollisionQueryParams TraceParams;

			GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1);
			if (Hit.bBlockingHit)
			{
				DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5),
					FColor::Red, false, 2.0f);
				
				CoverStateValue = FrontCover;
			}
				
			break; 
		}

		case FrontCover:
		{
			FVector Start = GetOwner()->GetActorLocation();
			//Start += FVector(100.f,0.f, 0.f);
			FVector End = Start + (GetOwner()->GetActorForwardVector() * (TraceDistence / 2));

			FCollisionQueryParams TraceParams;

			GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1);
			if (Hit.bBlockingHit)
			{
				DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5),
					FColor::Red, false, 2.0f);

				//CoverStateValue = FrontCover;
			}
			else
			{
				CoverStateValue = NoCover;
			}

			break;
		}
		 
		case InCover:
		{
			/*
			FVector Start = Character->GetActorLocation();
			FVector End = Start + ((GetOwner()->GetActorForwardVector() * (-1)) * (TraceDistence / 2));

			FCollisionQueryParams TraceParams;

			GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1);
			if (Hit.bBlockingHit)
			{
				DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5),
					FColor::Red, false, 2.0f);

				//CoverStateValue = NoCover;
			}
			else
			{
				//CoverStateValue = NoCover;
			}

			FHitResult HitLeft;
			FHitResult HitRight;



			FVector LeftLocation = Start + Character->GetCapsuleComponent()->GetRightVector() * 50.f;
				
			auto Capsule = Character->GetCapsuleComponent();
				
			FVector ForwardVector = Capsule->GetForwardVector();

			FVector EndHeatVector = LeftLocation - ForwardVector * TraceDistence;
	
				
			GetWorld()->LineTraceSingleByChannel(HitLeft, LeftLocation,
				EndHeatVector, ECC_Visibility, TraceParams);
			DrawDebugLine(GetWorld(), LeftLocation,
				EndHeatVector, FColor::Red, false, 0.1);

			if(HitLeft.bBlockingHit)
			{
				DrawDebugBox(GetWorld(), HitLeft.ImpactPoint, FVector(5, 5, 5),
					FColor::Red, false, 2.0f);
			}

			FVector ForwardVector = Capsule->GetForwardVector();
				
			FVector RightLocation = Start - Character->GetCapsuleComponent()->GetRightVector() * 50.f;

			FVector EndHeatVectorR = RightLocation - ForwardVector * TraceDistence;

				
			GetWorld()->LineTraceSingleByChannel(HitRight, RightLocation,
				EndHeatVectorR, ECC_Visibility, TraceParams);
			DrawDebugLine(GetWorld(), RightLocation,
				EndHeatVectorR, FColor::Red, false, 0.1);
	
			if(HitRight.bBlockingHit)
			{
				DrawDebugBox(GetWorld(), HitRight.ImpactPoint, FVector(5, 5, 5),
					FColor::Red, false, 2.0f);

				// todo вектор отражения
				auto Prikol = HitRight.ImpactPoint + (HitRight.Normal * 100);

				DrawDebugLine(GetWorld(), HitRight.ImpactPoint,
					Prikol, FColor::Yellow, false, 0.7);
				
				
				auto Prikol = EndHeatVectorR + HitRight.Normal;
				
				DrawDebugLine(GetWorld(), HitRight.ImpactPoint,
					Prikol, FColor::Yellow, false, 0.7);
				
			}
		*/

			FHitResult HitLeft = LefttVectorResultInCover();

			FHitResult HitRight = RightVectorResultInCover();
				
			break;
		}

		default :
		{
			break;
		}
	}

	// Угол отражения лайнтрейса, лайтрейс угол подения == угол отражения 
	
	/*
	
	FVector Loc;
	FRotator Rot;
	FHitResult Hit;
	
	FHitResult OutHit;
	
	FVector StartTrace = GetOwner()->GetActorLocation();
	FVector EndTrace = ((GetOwner()->GetActorForwardVector() * 60.f) + StartTrace);

	Character->GetController()->GetPlayerViewPoint(Loc, Rot);

	Character->GetActorForwardVector();

	FCollisionQueryParams Coll(true);

	//DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Green, false, 0.1);

	GetWorld()->LineTraceSingleByChannel(OutHit, StartTrace, EndTrace, ECC_Visibility);

	Character->ActorLineTraceSingle(OutHit, StartTrace, EndTrace, ECC_Visibility, Coll);

	*/
}

