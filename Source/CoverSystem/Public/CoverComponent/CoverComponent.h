// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoverComponent.generated.h"

class ACoverSystemCharacter;
class UCapsuleComponent;

enum CoverState : uint8
{
	NoCover = 0,
	FrontCover = 1,
	InCover = 2

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COVERSYSTEM_API UCoverComponent : public UActorComponent
{
	GENERATED_BODY()

	

public:	
	// Sets default values for this component's properties
	UCoverComponent();

	CoverState CoverStateValue;

protected:

	
	ACoverSystemCharacter * Character;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	float TraceDistence;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void CoverCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	 
	UFUNCTION() 
	void OnTakeCover();

	//FHitResult LetLineFront();
	
	//FHitResult LetLineBack();


	FHitResult LetLineFront(float value = 0.f);

	FHitResult LetLineBack(float value = 0.f);
	
	//FHitResult LetThighLineFront();

	//FHitResult LetThighLineBack();
	
	
	FHitResult RightVectorResultInCover(float value = 0.f);

	FHitResult LefttVectorResultInCover(float value = 0.f);

	TArray<FHitResult> IsTakeCover();

}; 

