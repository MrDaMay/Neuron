// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NR_EnvinronmentObjects.generated.h"

UCLASS()
class NEURON_API ANR_EnvinronmentObjects : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANR_EnvinronmentObjects();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class UStaticMeshComponent* StaticMesh = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Timers for change position start
	FTimerHandle ChangePositionTimer;
	FTimerHandle FinishChangePositionTimer;
	//Timers for change position end

	//Function for change position start
	UFUNCTION()
	void ChangeObjectForBoss();
	void ChangePosition();
	void FinishChangePosition();
	//Function for change position end

	//Variable for change Z position
	float ZChangeOffset;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
		float FinishZPosition;
};