// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Collectable/NR_CollectableInterface.h"
#include "NR_CollectableBase.generated.h"

UCLASS()
class NEURON_API ANR_CollectableBase : public AActor, public INR_CollectableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANR_CollectableBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class UStaticMeshComponent* StaticMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class USphereComponent* CollisionSphere = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class UParticleSystemComponent* LightPartical = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Implementing Interface
	void CollectItem_Implementation(ANR_Character* Character);
	//Interering an interactive zone
	void EnteredInteractionZone_Implementation(ANR_Character* Character); // Blueprint editable
	//Leaving an interactive zone
	void LeftInteractionZone_Implementation(ANR_Character* Character); // Blueprint editable

	UParticleSystem* OverlapSpawnPartical;

	FName ObjectName;
};
