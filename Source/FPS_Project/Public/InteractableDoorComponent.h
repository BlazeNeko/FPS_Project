// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "InteractableDoorComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FPS_PROJECT_API UInteractableDoorComponent : public UInteractableComponent
{
	GENERATED_BODY()


		UInteractableDoorComponent();
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InteractionDetails)
		bool bNeedsKey = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InteractionDetails)
		FString KeyName = "Key";

	//The position the door will have after interacting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InteractionDetails)
		float YawRotationAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InteractionDetails)
		float TimeToOpenDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InteractionDetails)
		bool bIsOpen = false;

private:

	//Tells if the door is opening or closing
	bool bIsInteracting = false;
	
	//Open the door
	void OpenDoor(float DeltaTime);

	//Close the door
	void CloseDoor(float DeltaTime);

	//Aux variables for opening and closing the door smoothly
	float DoorAlpha;	
	float DoorAlphaLerp;

	FRotator OpenRotation;
	FRotator ClosedRotation;

protected:

	//Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
		virtual bool Interact() override;

};
