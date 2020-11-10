// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_Character.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractableComponent.h"
#include "Pickup.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"


// Sets default values
AFPS_Character::AFPS_Character()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//Create Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, CurrentEyeHeight));
	CameraComponent->bUsePawnControlRotation = true;

	// Create Health Component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	// Create Inventory Component
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	//Enable crouching
	UCharacterMovementComponent* Movement = (UCharacterMovementComponent*)GetMovementComponent();
	Movement->NavAgentProps.bCanCrouch = true;
	
	//Calculate eye position when crouching
	CrouchRealEyeHeight = BaseEyeHeight - (BaseEyeHeight * 2 - CrouchedEyeHeight * 2);

	//Initialize FOV
	CurrentFOV = CameraComponent->FieldOfView;



}

// Called when the game starts or when spawned
void AFPS_Character::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void AFPS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SmoothCrouch(DeltaTime);
	AdjustFOV(DeltaTime);

	Inspect();

}

// Called to bind functionality to input
void AFPS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Axis Bindings
	InputComponent->BindAxis("MoveForward", this, &AFPS_Character::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFPS_Character::MoveRight);
	InputComponent->BindAxis("CameraPitch", this, &AFPS_Character::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &AFPS_Character::YawCamera);

	//Action Bindings
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AFPS_Character::Sprint);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &AFPS_Character::SprintReleased);
	
	UCharacterMovementComponent* Movement = (UCharacterMovementComponent*)GetMovementComponent();

	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AFPS_Character::CrouchStarted);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Released, this, &AFPS_Character::CrouchReleased);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AFPS_Character::Jump);
	InputComponent->BindAction("Zoom", EInputEvent::IE_Pressed, this, &AFPS_Character::Zoom);
	InputComponent->BindAction("Zoom", EInputEvent::IE_Released, this, &AFPS_Character::ZoomReleased);
	InputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &AFPS_Character::Interact);




}
//Apply movement forward/backwards
void AFPS_Character::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);

}

//Apply movement left/right
void AFPS_Character::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue );


}

//Rotate camera pitch
void AFPS_Character::PitchCamera(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationSpeed * GetWorld()->GetDeltaSeconds());


}

//Rotate camera yaw
void AFPS_Character::YawCamera(float AxisValue)
{
	
	AddControllerYawInput(AxisValue * RotationSpeed * GetWorld()->GetDeltaSeconds());


}

//Start sprinting
void AFPS_Character::Sprint()
{

	//Safety check in case we receive 2 inputs
	if (!bIsSprinting && !bIsCrouching && !bIsZooming) {
		UCharacterMovementComponent* Movement = (UCharacterMovementComponent*)GetMovementComponent();

		Movement->MaxWalkSpeed *= SprintSpeedRatio;

		bIsSprinting = true;
	}
}

//Stop sprinting
void AFPS_Character::SprintReleased()
{
	//Safety check in case we receive 2 inputs

	if (bIsSprinting) {
		UCharacterMovementComponent* Movement = (UCharacterMovementComponent*)GetMovementComponent();

		Movement->MaxWalkSpeed /= SprintSpeedRatio;

		bIsSprinting = false;
	}
}

//Start crouching
void AFPS_Character::CrouchStarted()
{
	//Safety check in case we receive 2 inputs
	if (!bIsCrouching) {

		UCharacterMovementComponent* Movement = (UCharacterMovementComponent*)GetMovementComponent();

		Movement->Crouch();


		bIsCrouching = true;


	}
}

//Stop crouching
void AFPS_Character::CrouchReleased()
{
	if (bIsCrouching) {
		UCharacterMovementComponent* Movement = (UCharacterMovementComponent*)GetMovementComponent();

		Movement->UnCrouch();

		bIsCrouching = false;

	}
}

void AFPS_Character::Zoom()
{
	
	if (!bIsZooming) {

		bIsZooming = true;


	}
}

void AFPS_Character::ZoomReleased() 
{

	if (bIsZooming) {

		bIsZooming = false;


	}
}

void AFPS_Character::Interact()
{
	if (bValidInspection) {

		UInteractableComponent* Interactable = Hit.Actor->FindComponentByClass<UInteractableComponent>();

		switch (inspectedType) {
		case 2:

			Interactable->Interact();
			break;

		default:

			break;
		}
	}
}

void AFPS_Character::Inspect() 
{
	//Initialization of line trace params
	static bool InteractableDetected = false;

	FCollisionQueryParams params(
		FName(TEXT("")),				//trace tag
		false,							//false == simple traces, true = complex traces
		GetOwner()						//Objects to ignore
	);
	FVector startLocation = CameraComponent->GetComponentLocation();
	InteractableDetected = GetWorld()->LineTraceSingleByObjectType(
		Hit,																				//FHitResult
		startLocation,																	//start position of the ray, FVector
		startLocation + CameraComponent->GetForwardVector() * MaxInspectDistance,		//end position of the ray, FVector
		FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel2),				//Collision filter (only Interactable items, which includes the interactable component and pickups)
		params																				//parameters defined previously
	);
	if (InteractableDetected) {
		//Check if the actor detected is a pickup 
		APickup* Pickup = Cast<APickup>(Hit.Actor);
		UInteractableComponent* Interactable = Hit.Actor->FindComponentByClass<UInteractableComponent>();

		if (Pickup != nullptr)
		{
			InspectedItemName = Pickup->Name;
			bValidInspection = true;
			inspectedType = 1;

		}
		else if (Interactable != nullptr) {

			InspectedItemName = Interactable->Name;
			bValidInspection = true;
			inspectedType = 2;
		}
		else {
			bValidInspection = false;
			inspectedType = 0;
		}

	//	UE_LOG(LogTemp, Warning, TEXT("Actor Detected %s"), *InspectedItemName);

	}else {
		bValidInspection = false;
		inspectedType = 0;
	}

	DisplayWidget();
	
	if (false) {
		DrawDebugLine(GetWorld(), startLocation, startLocation + CameraComponent->GetForwardVector() * MaxInspectDistance, FColor(135, 0, 145), false, 0.f, 0.f, 1);
	}
}

void AFPS_Character::DisplayWidget()
{
	if (bValidInspection) {
		if (bIsInspectionWidgetCreated) {
			
		}
		else {
			//create the widget
			InspectionWidget = CreateWidget<UUserWidget>(GetWorld(), HoverActorsHUD);
			//Add widget to viewport
			if (ensure(InspectionWidget != nullptr)) {
				InspectionWidget->AddToViewport();
			}
			bIsInspectionWidgetCreated = true;
		}

	}
	else {
		if (bIsInspectionWidgetCreated) {
			InspectionWidget->RemoveFromParent();
			bIsInspectionWidgetCreated = false;
		}
	}

}

void AFPS_Character::AdjustFOV(float DeltaTime) 
{
	//Zoom in
	if (bIsZooming && CurrentFOV - ZoomedFOV> 0.f) {

		ZoomAlpha += DeltaTime;
		ZoomAlpha = FMath::Clamp(ZoomAlpha, 0.f, ZoomTransitionTime); //Time Interval for crouching

		ZoomAlphaLerp = ZoomAlpha / CrouchTransitionTime;
		ZoomAlphaLerp = ZoomAlphaLerp * ZoomAlphaLerp; //quadratic function for smoothness (x^2)


		CurrentFOV = FMath::Lerp(FOV, ZoomedFOV, ZoomAlphaLerp); //Calculate eye height
		CameraComponent->FieldOfView = CurrentFOV; //Adjust eye height

		if (bIsSprinting) {
			SprintReleased();
		}

	}//Zoom out
	else if (!bIsZooming && FOV - CurrentFOV > 0.f) {

		//Calculate interpolation alpha
		ZoomAlpha -= DeltaTime;
		ZoomAlpha = FMath::Clamp(ZoomAlpha, 0.f, CrouchTransitionTime); //Time Interval for zooming

		ZoomAlphaLerp = ZoomAlpha / ZoomTransitionTime;
		ZoomAlphaLerp = ZoomAlphaLerp * ZoomAlphaLerp; //quadratic function for smoothness (x^2)


		CurrentFOV = FMath::Lerp(FOV, ZoomedFOV, ZoomAlphaLerp); //Calculate FOV
		CameraComponent->FieldOfView = CurrentFOV; //Adjust FOV
	}


}

//Calculate camera pos at Crouch/Uncrouch
void AFPS_Character::SmoothCrouch(float DeltaTime) 
{

	
	//Crouch
	if (bIsCrouching && CurrentEyeHeight - CrouchRealEyeHeight > 0.f)  {

		CrouchAlpha += DeltaTime;
		CrouchAlpha = FMath::Clamp(CrouchAlpha, 0.f, CrouchTransitionTime); //Time Interval for zooming

		CrouchAlphaLerp = CrouchAlpha / CrouchTransitionTime;
		CrouchAlphaLerp = CrouchAlphaLerp * CrouchAlphaLerp ; //quadratic function for smoothness (x^2)


		CurrentEyeHeight = FMath::Lerp(BaseEyeHeight, CrouchRealEyeHeight, CrouchAlphaLerp); //Calculate FOV
		CameraComponent->SetRelativeLocation(FVector(CameraComponent->GetRelativeLocation().X, CameraComponent->GetRelativeLocation().Y, CurrentEyeHeight)); //Adjust FOV

		if (bIsSprinting) {
			SprintReleased();
		}

	}//Uncrouch
	else if (!bIsCrouching && BaseEyeHeight - CurrentEyeHeight > 0.f) {

		//Calculate interpolation alpha
		CrouchAlpha -= DeltaTime;
		CrouchAlpha = FMath::Clamp(CrouchAlpha,0.f, CrouchTransitionTime); //Time Interval for crouching

		CrouchAlphaLerp = CrouchAlpha / CrouchTransitionTime;
		CrouchAlphaLerp = CrouchAlphaLerp * CrouchAlphaLerp ; //quadratic function for smoothness (x^2)


		CurrentEyeHeight = FMath::Lerp(BaseEyeHeight, CrouchRealEyeHeight, CrouchAlphaLerp); //Calculate eye height
		CameraComponent->SetRelativeLocation(FVector(CameraComponent->GetRelativeLocation().X, CameraComponent->GetRelativeLocation().Y, CurrentEyeHeight)); //Adjust eye height
	}
}



