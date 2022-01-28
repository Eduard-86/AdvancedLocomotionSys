

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoverComponent.generated.h"


UENUM(BlueprintType)
enum class CoverState : uint8
{
	State = 0,
	//FrontCover = 1,
	Crouch = 1

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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NewMoveCover")
	float height_newmoveR = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NewMoveCover")
	float width_newmoveR = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NewMoveCover")
	float height_newmoveL = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NewMoveCover")
	float width_newmoveL = 0;

protected:

	ACharacter* Character;

	UCharacterMovementComponent* CharacterMovement;
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	float TraceDistence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	float CoverTraceHeightState = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	float CoverTraceHeightCrouch = 40.f;

	float CoverTraceHeightOnMove = CoverTraceHeightState;

	FVector_NetQuantizeNormal CoverNormal;
	
public:	
	
	FHitResult LetLineFront(float value = 0.f);

	FHitResult LetLineBack(float value = 0.f);

	FHitResult RightVectorResultInCover(float height = 0.f, float width = 0.f);

	FHitResult LefttVectorResultInCover(float height = 0.f, float width = 0.f);

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
	bool MoveRightInCover(float Value, float TraceHeught);

	UFUNCTION(BlueprintCallable, Category = "Cover")
	bool ControlIsTakeCover();

	UFUNCTION(BlueprintCallable, Category = "Cover")
	void SetDefaultValueCoverTraceHeightMoveState();

	UFUNCTION(BlueprintCallable, Category = "Cover")
	void SetDefaultValueCoverTraceHeightMoveCrouch();

	UFUNCTION(BlueprintCallable, Category = "Cover")
	float ReturnDifference();

#pragma region newlaw
	
	UFUNCTION(BlueprintCallable, Category = "Cover")
	FVector Test();

	UFUNCTION(BlueprintCallable, Category = "Trace") 
	bool LineTrace(float height = 0.f, float width = 0.f);

	UFUNCTION(BlueprintCallable, Category = "Cover")
	bool MoveInCoverForward_New(float Value, float TraceHeught);

	UFUNCTION(BlueprintCallable, Category = "Cover")
	bool MoveInCoverRight_New(float Value, float TraceHeught);

	UFUNCTION(BlueprintCallable, Category = "Cover")
	void SetStateEnum(bool key);

	
#pragma endregion 
	
	
#pragma endregion 
};
