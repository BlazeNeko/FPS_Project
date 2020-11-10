// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "KeyPickup.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API AKeyPickup : public APickup
{
	GENERATED_BODY()
	
		AKeyPickup();

	virtual void UseItem() override;
};
