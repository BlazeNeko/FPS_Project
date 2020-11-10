// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

//Method for debugging the inventory
void UInventoryComponent::PrintInventory()
{
	UE_LOG(LogTemp, Warning, TEXT("----------- INVENTORY ----------"));

	for (auto i : Inventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s x%f"), *i->Name, i->Amount);
	}
	UE_LOG(LogTemp, Warning, TEXT("--------------------------------"));

}

void UInventoryComponent::UpdateInventory()
{
	OnUpdateInventory.Broadcast(Inventory);
}

bool UInventoryComponent::AddToInventory(APickup* Item)
{

	bool success = false;
	//If the item is already in the inventory, increase the amount
	for (APickup* i : Inventory)
	{

		if (i->Name == Item->Name) {

			i->Amount += Item->Amount;
			Item->Destroy();
			success = true;
		}
		
	}
	if (Inventory.Num() < InventorySize && !success) {	//If the item wasn't in the inventory and the inventory is not full, add it.

		Inventory.Add(Item);

		success = true;
	}

	UpdateInventory();

	return success;
}

int32 UInventoryComponent::HasItem(FString name) {

	int32 amount = 0;
	//Iterate the inventory
	for (auto i : Inventory) {
		if (i->Name == name) {
			amount = i->Amount;
			
			break;
		}
	}
	return amount;
}


int32 UInventoryComponent::RemoveFromInventory(FString name, int32 amount)
{
	int32 amountRemoved = 0;
	//If the item is already in the inventory, decrease the amount
	for( auto i: Inventory){
		if (i->Name == name) {

			//amount that will be removed
			amountRemoved = FMath::Min(amount, i->Amount);
			
			i->Amount -= amount;


			//if the remaining amount is 0, remove the item.
			if (i->Amount <= 0) {
				Inventory.Remove(i);
				i->Destroy();
			}
			UpdateInventory();
			break;
		}
	}
	return amountRemoved;
}

TArray<APickup*> UInventoryComponent::GetInventory()
{

	return Inventory;
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

