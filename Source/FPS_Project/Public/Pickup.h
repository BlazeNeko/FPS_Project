// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Texture2D.h"
#include "Pickup.generated.h"

UCLASS()
class FPS_PROJECT_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	APickup(FString name, int32 amount);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Capsule, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
		float ColliderRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		int32 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		bool bIsUsable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UTexture2D* Icon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	UFUNCTION()
	void OnDelegateBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable)
		virtual void UseItem() PURE_VIRTUAL(APickup::UseItem);

	FORCEINLINE bool operator == (const APickup Other) {
		if (Name == Other.Name)
			return true;
		return false;
	}
	FORCEINLINE bool operator == (const APickup* Other) {
		if (Name == Other->Name)
			return true;
		return false;
	}
};
