// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Engine/World.h"

#include "MedicinePickup.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API AMedicinePickup : public APickup
{
	GENERATED_BODY()

	AMedicinePickup();

	virtual void UseItem() override;
	
};
