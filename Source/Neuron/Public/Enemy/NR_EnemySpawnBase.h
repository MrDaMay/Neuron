// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Enemy/NR_EnemyCharacterBase.h"
#include "NR_EnemySpawnBase.generated.h"

UCLASS()
class NEURON_API ANR_EnemySpawnBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANR_EnemySpawnBase();

	FORCEINLINE class UBoxComponent* GetBoxComponent() const { return BoxComponent; }

private:

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SpawnEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ANR_EnemyCharacterBase> Enemy;
};
