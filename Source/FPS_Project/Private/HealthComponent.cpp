// Fill out your copyright notice in the Description page of Project Settings.



#include "HealthComponent.h"
#include "FPS_Character.h"
#include "Templates/Casts.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::ReceiveDamage(float Damage) {
	HealthPoints -= Damage;
	if (HealthPoints <= 0)
		Death();
}

bool UHealthComponent::RestoreHealth(float Health)
{
	if (HealthPoints < MaxHealth) {
		HealthPoints += Health;
		return true;
	}
	return false;

}

void UHealthComponent::Death()
{
	
	//Get Player Controller
	auto PC = GetWorld()->GetFirstPlayerController();
	if (!ensure(PC != nullptr)) return;

	//Set Game Paused
	PC->SetPause(true);

	//Create Widget
	DeathWidget = CreateWidget<UUserWidget>(GetWorld(), DeathScreenHUD);
	DeathWidget->AddToViewport();

	// Set up Input Mode and widget to focus
	FInputModeUIOnly InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetWidgetToFocus(DeathWidget->TakeWidget()); 
	PC->SetInputMode(InputModeData);

	// Enable Mouse Cursor
	PC->bShowMouseCursor = true;
}
