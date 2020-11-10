// Fill out your copyright notice in the Description page of Project Settings.


#include "MedicinePickup.h"
#include "HealthComponent.h"
#include "FPS_Character.h"
#include "InventoryComponent.h"

AMedicinePickup::AMedicinePickup() {

	Super::bIsUsable = true;
}



void AMedicinePickup::UseItem()
{
	AFPS_Character* Character;
	UInventoryComponent* Inventory;

	Character = Cast<AFPS_Character>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	Inventory = Character->GetInventoryComponent();
	if (Character->GetHealthComponent()->RestoreHealth(1) && Inventory != nullptr) {
		Inventory->RemoveFromInventory(Name, 1);
	}
}
