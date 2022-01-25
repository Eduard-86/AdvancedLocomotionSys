

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoverComponent.generated.h"


UENUM(BlueprintType)
enum class CoverState : uint8
{
	NoCover = 0,
	//FrontCover = 1,
	InCover = 1

};

class UCharacterMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVANCEDLOCOMOTIONSYSTEMV_API UCoverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCoverComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Zoom")
	CoverState CoverStateValue;

	bool InCover = false;

protected:

	ACharacter* Character;

	UCharacterMovementComponent* CharacterMovement;
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	float TraceDistence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	float CoverTraceHeight = 150.f;

public:	
	
	FHitResult LetLineFront(float value = 0.f);

	FHitResult LetLineBack(float value = 0.f);

	FHitResult RightVectorResultInCover(float value = 0.f);

	FHitResult LefttVectorResultInCover(float value = 0.f);

	TArray<FHitResult> IsTakeCover();

public:

#pragma region	Control

	FInputAxisBinding AxisForword;

	FInputAxisBinding AxisRight;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//////////////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "Movable")
	bool MoveForwardInCover(float Value);

	UFUNCTION(BlueprintCallable, Category = "Movable")
	bool MoveRightInCover(float Value);

	UFUNCTION(BlueprintCallable, Category = "Cover")
	bool ControlIsTakeCover();


	
	
#pragma endregion 
};
