// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Enemy/NR_EnemyCharacterBase.h"
#include "NR_EnemySpawnBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossSpawn);

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

	//GameState reference
	class ANR_GameState* MyGameState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Calls Remaining
	float RepeatingCallsRemaining = 0.f;
	
	//Spawn Timer
	FTimerHandle SpawnEnemyTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Delegate for spawn ending
	FOnSpawnEnd OnSpawnEnd;

	FOnBossSpawn OnBossSpawned;

	//Spawn details
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Details")
		float SpawnRate = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Details")
		float EnemiesNumber = 15.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Details")
		float SpawnDelay = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Details")
		TArray<TSubclassOf<ANR_EnemyCharacterBase>> Enemy;

	UFUNCTION()
		void SpawnBoss();
	UFUNCTION()
		void SpawnEnemy();
	UFUNCTION()
		void StartSpawn();
	UFUNCTION()
		void StopSpawn();

};
