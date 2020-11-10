// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "Components/SphereComponent.h"
#include "FPS_Character.h"
#include "InventoryComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));

	SphereComponent->SetSphereRadius(250.f);
	SphereComponent->SetHiddenInGame(true);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnDelegateBeginOverlap);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(SphereComponent);

	
}

APickup::APickup(FString name, int32 amount) : Name(name), Amount(amount){

}


// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}


void APickup::OnDelegateBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPS_Character* Character = Cast<AFPS_Character>(OtherActor);

	UInventoryComponent* Inventory = Character->FindComponentByClass<UInventoryComponent>();
	if (Inventory != nullptr) {
		Inventory->AddToInventory(this);

	}

	StaticMeshComponent->DestroyComponent();
	SphereComponent->DestroyComponent();
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


