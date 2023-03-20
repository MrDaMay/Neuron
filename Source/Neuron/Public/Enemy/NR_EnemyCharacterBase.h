// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Component/NR_HealthComponent.h"
#include "Component/NR_KillScoreComponent.h"
#include "Enemy/NR_FreezeInterface.h"
#include "Character/NR_Character.h"
#include "NR_EnemyCharacterBase.generated.h"

UCLASS()
class NEURON_API ANR_EnemyCharacterBase : public ACharacter, public INR_FreezeInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANR_EnemyCharacterBase();

	//HealthComponent
	FORCEINLINE class UNR_HealthComponent* GetHealthComponent() const { return HealthComponent; }
	//KillScoreComponent
	FORCEINLINE class UNR_KillScoreComponent* GetKillScoreComponent() const { return KillScoreComponent; }

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
		class UNR_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = KillDCore, meta = (AllowPrivateAccess = "true"))
		class UNR_KillScoreComponent* KillScoreComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		bool CanMovePawn = true;
	UPROPERTY()
		bool CanAttackPawn = true;

	UAnimInstance* AnimAttackInstance;

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Combat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float BaseAttackDamage = 30.0f;
	UPROPERTY(BlueprintReadWrite, Category = "Damage")
		float RadiusMeleeAttack = 30.0f;

	//AI
	UFUNCTION(BlueprintCallable)
		bool CanEnemyAttack();
	UFUNCTION(BlueprintCallable)
		bool CanEnemyMove();
	UFUNCTION(BlueprintCallable)
		virtual	void StartAttackEnemy();


	//Damage
	UFUNCTION()
		void HealthChange(float Health, float Damage);
	UFUNCTION(BlueprintNativeEvent)
		void HealthChange_BP(float Health, float Damage);
	UPROPERTY(BlueprintReadWrite, Category = "Damage")
		float DistanceToAttack = 150.0f;

	//Dead
	UFUNCTION()
		virtual void CharacterDead();
	UFUNCTION(BlueprintNativeEvent)
		void CharacterDead_BP();


	//Montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
		UAnimMontage* DeadMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
		UAnimMontage* AttackMontage = nullptr;

	//State
	UFUNCTION(BlueprintCallable)
		void SetStateAttack(bool CanAttack);
	UFUNCTION(BlueprintCallable)
		void SetStateMove(bool CanMove);

	//Attack
	UFUNCTION()
		virtual void AttackAnimEnd(UAnimMontage* Montage, bool bIsSuccess);
	UFUNCTION()
		virtual	void AttackEnemy(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	//Interface freeze start
	virtual void Freeze(float FreezeTime) override;
	FTimerHandle FreezeTimerHandle;
	void EndFreeze();
	//Interface freeze end

	//Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MaxSpeed = 400.0f;

	//Sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* DeathSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* SpawnSound;

	//Drop start
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	TArray<FName> DropObjectNames;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	float DropChance;
	virtual void Drop();
	//Drop end
};
