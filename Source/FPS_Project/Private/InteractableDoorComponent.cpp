// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableDoorComponent.h"

#include "FPS_Character.h"
#include "InventoryComponent.h"
#include "KeyPickup.h"

UInteractableDoorComponent::UInteractableDoorComponent() {

	Super::Name = "Door";
}

void UInteractableDoorComponent::BeginPlay()
{
	Super::BeginPlay();

	//Initialize the open and closed rotation according to the initial state
	FRotator InitialPosition = GetOwner()->GetActorRotation();
	OpenRotation = bIsOpen ? InitialPosition : InitialPosition + FRotator(0.f, YawRotationAngle, 0.f);
	ClosedRotation = !bIsOpen ? InitialPosition : InitialPosition + FRotator(0.f, YawRotationAngle, 0.f);

	DoorAlpha = bIsOpen ? TimeToOpenDoor : 0.f;
}

void UInteractableDoorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	OpenDoor(DeltaTime);
	CloseDoor(DeltaTime);
}

void UInteractableDoorComponent::OpenDoor(float DeltaTime)
{
	//If the door is closed/closing  
	if (!bIsOpen && bIsInteracting ) {
		FRotator CurrentRotation = GetOwner()->GetActorRotation();

		//if the door rotation hasn't reached the closed state, interpolate its rotation
		if (FMath::Abs(CurrentRotation.Yaw - OpenRotation.Yaw) > 0.1f) {
			UE_LOG(LogTemp, Warning, TEXT("uwuwuwu"));

			DoorAlpha += DeltaTime;
			DoorAlpha = FMath::Clamp(DoorAlpha, 0.f, TimeToOpenDoor); //Time Interval for opening the door

			DoorAlphaLerp = DoorAlpha / TimeToOpenDoor;
			DoorAlphaLerp = DoorAlphaLerp;					

			CurrentRotation.Yaw = FMath::Lerp(ClosedRotation.Yaw, OpenRotation.Yaw, DoorAlphaLerp); //Calculate FOV
			GetOwner()->SetActorRotation(CurrentRotation);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("uwu"));
			bIsOpen = true;
			bIsInteracting = false;

		}
	}
}

void UInteractableDoorComponent::CloseDoor(float DeltaTime)
{
	//If the door is open/opening  
	if (bIsOpen && bIsInteracting ) {
		FRotator CurrentRotation = GetOwner()->GetActorRotation();

		//if the door rotation hasn't reached the closed state, interpolate its rotation
		if (FMath::Abs(CurrentRotation.Yaw - ClosedRotation.Yaw) > 0.1f) {

			DoorAlpha -= DeltaTime;
			DoorAlpha = FMath::Clamp(DoorAlpha, 0.f, TimeToOpenDoor); //Time Interval for opening the door

			DoorAlphaLerp = DoorAlpha / TimeToOpenDoor;
			DoorAlphaLerp = DoorAlphaLerp;

			CurrentRotation.Yaw = FMath::Lerp(ClosedRotation.Yaw, OpenRotation.Yaw, DoorAlphaLerp); //Calculate FOV
			GetOwner()->SetActorRotation(CurrentRotation);
		}
		else {
			bIsOpen = false;
			bIsInteracting = false;
		}
	}
}





bool UInteractableDoorComponent::Interact()
{

	AFPS_Character* Character = Cast<AFPS_Character>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	UInventoryComponent* Inventory = Character->GetInventoryComponent();

	if (bNeedsKey && Inventory->HasItem("Key")) {

		bIsInteracting = true;
		bNeedsKey = false;
		Inventory->RemoveFromInventory("Key", 1);
		return true;
	}
	else if (!bNeedsKey) {
		bIsInteracting = true;
		return true;
	}
	return false;
}


