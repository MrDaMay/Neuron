// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Component/NR_HealthComponent.h"
#include "Component/NR_TokenComponent.h"
#include "Weapon/NR_Weapon.h"
#include "FuncLibrary/Type.h"
#include "NR_Character.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponParamsChange, float, CoefFireSpeed, float, CoefDamage);

UCLASS()
class NEURON_API ANR_Character : public ACharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//Stats start
	float CoefMovementSpeed = 1.0f;
	float CoefFireSpeed = 1.0f;
	float CoefDamage = 1.0f;
	bool Immortality = false;
	float BaseSpeed = 600.0f;
	//Stats end

	//Inventory system start
	TArray<FName> WeaponSLot;
	//Inventory system end

	//Input start
	float AxisX = 0.0f;
	float AxisY = 0.0f;
	//Input end

public:
	// Sets default values for this character's properties
	ANR_Character();

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns HealthComponent subobject **/
	FORCEINLINE class UNR_HealthComponent* GetHealthComponent() const { return HealthComponent; }
	/** Returns HealthComponent subobject **/
	FORCEINLINE class UNR_TokenComponent* GetTokenComponent() const { return TokenComponent; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
		class UNR_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
		class UNR_TokenComponent* TokenComponent;

	//Character stats for achievements and tokens implementation
	FCharStats Stats;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Delegate start
	FOnWeaponParamsChange OnWeaponParamsChange;
	//Delegate end

	//Cursor start
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
		UMaterialInterface* CursorMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
		FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);
	UDecalComponent* CurrentCursor = nullptr;
	//Cursor end

	//Input start
	UFUNCTION()
		void InputAxisX(float Value);
	UFUNCTION()
		void InputAxisY(float Value);
	UFUNCTION()
		void InputAttackPressed();
	UFUNCTION()
		void InputAttackReleased();
	//Input end

	//Weapon start
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		ANR_Weapon* CurrentWeapon = nullptr;
	UFUNCTION(BlueprintCallable)
		ANR_Weapon* GetCurrentWeapon();
	UFUNCTION(BlueprintCallable)
		void InitWeapon(FName IdWeaponName);
	UFUNCTION()
		void WeaponFireStart(UAnimMontage* Anim);
	UFUNCTION(BlueprintNativeEvent)
		void WeaponFireStart_BP(UAnimMontage* Anim);
	UFUNCTION(BlueprintCallable)
		void StartSwitchWeapon();
	UFUNCTION(BlueprintNativeEvent)
		void StartSwitchWeapon_BP();
	UFUNCTION(BlueprintCallable)
		void EndSwitchWeapon();
	UFUNCTION(BlueprintNativeEvent)
		void EndSwitchWeapon_BP();
	//ToDo OverlapPickUpWeapon = nullptr;
	//Weapon end

	//Tick function start
	UFUNCTION()
		void MovementTick(float DeltaTime);
	//Tick function end

	//Shoot start
	FVector MouseLocation;
	FVector LineEnd;
	UFUNCTION(BlueprintCallable)
		void AttackEvent(bool bIsFire);
	//Shoot end

	//Dead start
	UFUNCTION()
		void CharDead();
	UFUNCTION(BlueprintNativeEvent)
		void CharDead_BP();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage")
		UAnimMontage* DeathMontage;
	UFUNCTION()
		void AbsolutelyDead(bool IsWin);
	//Dead end

	//Bonus start
	UFUNCTION(BlueprintCallable, Category= "Bonus")
	void TakeBonus(EBonusType BonusType);
	FTimerHandle FinishFireBonusTimerHamdle;
	void FinishFireBonus();
	FTimerHandle FinishImmortalityBonusTimerHamdle;
	void FinishImmortalityBonus();
	FTimerHandle FinishMovementSpeedBonusTimerHamdle;
	void FinishMovementSpeedBonus();
	void FreezeBonusFunction();
	//Bonus end

	//Inventory system start
	UFUNCTION(BlueprintCallable, Category="Weapon")
		void AddNewWeaponToWeaponSlot(FName NewWeapon);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		TArray<FName> GetWeaponSlot();
	//Inventory system end
	//Function for updating stats after applying tokens
	UFUNCTION()
		void UpdateStats(FCharStats NewStats);

	//Stats start
	//float CoefMovementSpeed = 1.0f;
	//float CoefFireSpeed = 1.0f;
	//float CoefDamage = 1.0f;
	//bool Immortality = false;
	//float BaseSpeed = 600.0f;
	//Stats end
};