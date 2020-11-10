// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Array.h"
#include "Pickup.h"
#include "InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateInventoryDelegate, const TArray <APickup*>&, InventoryItems);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_PROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Inventory Storage
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<APickup*> Inventory;

	//Inventory Size
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 InventorySize = 12;

public:	

	UFUNCTION(BlueprintCallable)
		void PrintInventory();
	
	//Function to refresh the inventory UI
	UFUNCTION(BlueprintCallable)
		void UpdateInventory();

	//Add pickup to inventory
	UFUNCTION(BlueprintCallable)
		bool AddToInventory(APickup* Item);

	//check if the inventory contains a certain item
	UFUNCTION(BlueprintCallable)
		int32 HasItem(FString name);

	//Remove pickup from inventory
	UFUNCTION(BlueprintCallable)
		int32 RemoveFromInventory(FString name, int32 amount);

	UFUNCTION(BlueprintCallable)
		TArray<APickup*> GetInventory();

	UPROPERTY(BlueprintAssignable, Category = "Pickup")
		FUpdateInventoryDelegate OnUpdateInventory;


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
