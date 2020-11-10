// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"

#include "FPS_Character.generated.h"

class UUserWidget;

UCLASS()
class FPS_PROJECT_API AFPS_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFPS_Character();

protected:

	//Components
	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CameraComponent;

	// Health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
		class UHealthComponent* HealthComponent;

	//Inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
		class UInventoryComponent* InventoryComponent;

	//Interaction HUD Widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> HoverActorsHUD;

	//Input 
	FVector2D MovementInput;
	FVector2D CameraInput;

	//Speed params
	UPROPERTY(EditAnywhere)
		float RotationSpeed = 30;
	UPROPERTY(EditAnywhere)
		float SprintSpeedRatio = 3;
	
	//Camera params
	UPROPERTY(EditAnywhere, Category="Camera")
		float FOV = 90;
	UPROPERTY(EditAnywhere, Category="Camera")
		float ZoomedFOV = 45;
	UPROPERTY(EditAnywhere, Category = "Camera")
		float ZoomTransitionTime = 0.3;

	//Crouch Params
	UPROPERTY(EditAnywhere, Category = "Crouch")
		float CrouchTransitionTime = 0.3;

	//Interaction Params
	UPROPERTY(EditAnywhere, Category = "Interaction")
		float MaxInspectDistance = 300;

	float CurrentCapsuleHeight;


public:
	//Flags
	UPROPERTY(BlueprintReadOnly, Category = Flags)
		bool bIsSprinting;
	UPROPERTY(BlueprintReadOnly, Category = Flags)
		bool bIsCrouching;
	UPROPERTY(BlueprintReadOnly, Category = Flags)
		bool bIsZooming;

		bool bIsRecoveringFromCrouch;


private:

	//Aux variables for crouch smooth interpolation
	float CrouchAlpha = 0;
	float CrouchAlphaLerp = 0;
	float CrouchRealEyeHeight;
	float CurrentEyeHeight = BaseEyeHeight;

	//Aux variables for zoom smooth FOV interpolation
	float ZoomAlpha = 0;
	float ZoomAlphaLerp = 0;
	float CurrentFOV;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Input functions
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);

	void Sprint();
	void SprintReleased();

	void CrouchStarted();
	void CrouchReleased();

	void Zoom();
	void ZoomReleased();

	void Interact();

	//tick functions based on interpolations for smoothness
	void SmoothCrouch(float DeltaTime);
	void AdjustFOV(float DeltaTime);	

	//Get Components
	UFUNCTION(BlueprintCallable)
		FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE class UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE class UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

private:

	//Line traces in front to detect items
	void Inspect();

	//Displays information about the interactable items detected by the Inspect method
	void DisplayWidget();
	
	UUserWidget* InspectionWidget;
	bool bIsInspectionWidgetCreated;
	//Hit Results of the inspection
	FHitResult Hit;
	bool bValidInspection;
	int32 inspectedType = 0;  // 0 = none, 1 = pickup, 2 = interactable

public:

	UPROPERTY(BlueprintReadOnly, Category = "Inspection")
		FString InspectedItemName;



};