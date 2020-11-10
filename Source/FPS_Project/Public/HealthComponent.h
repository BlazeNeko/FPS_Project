// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"

#include "Components/ActorComponent.h"
#include "Templates/SubclassOf.h"

#include "HealthComponent.generated.h"

class UUserWidget;

UCLASS(DefaultToInstanced, BlueprintType, ClassGroup=(Health), meta=(BlueprintSpawnableComponent) )
class FPS_PROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		int HealthPoints = 3;

	UPROPERTY(EditAnywhere)
		int MaxHealth = 3;
	//-------------------------------------------------------------
	//---------------------------- HUD ----------------------------
	//-------------------------------------------------------------
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<UUserWidget> DeathScreenHUD;

	//UPROPERTY()
		UUserWidget* DeathWidget;
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		//-------------------------------------------------------------

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Receive Damage
	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(float Damage);

	//Heal HP
	UFUNCTION(BlueprintCallable)
	bool RestoreHealth(float Health);

	//Kill owner
	void Death();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE  int GetHealthPoints() const { return HealthPoints; }
}; 
