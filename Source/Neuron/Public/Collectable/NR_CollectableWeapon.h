// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Collectable/NR_CollectableInterface.h"
#include "NR_CollectableWeapon.generated.h"

UCLASS()
class NEURON_API ANR_CollectableWeapon : public AActor, public INR_CollectableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANR_CollectableWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class USceneComponent* SceneComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class UStaticMeshComponent* StaticMeshWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class USphereComponent* CollisionSphere = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Weapon info
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	FName WeaponName = "None";

	//Init FName variable
	void Init(FName Name) { WeaponName = Name; };

	//Interering an interactive zone
	void EnteredInteractionZone_Implementation(ANR_Character* Character) override; // c++ implementation

};
