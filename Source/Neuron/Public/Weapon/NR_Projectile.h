// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FuncLibrary/Type.h"
#include "GameFramework/Actor.h"
#include "NR_Projectile.generated.h"


UCLASS()
class NEURON_API ANR_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANR_Projectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class UStaticMeshComponent* BulletMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class USphereComponent* BulletCollisionSphere = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class UProjectileMovementComponent* BulletProjectileMovement = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
		class UParticleSystemComponent* BulletFX = nullptr;

	UPROPERTY(BlueprintReadOnly)
		FProjectileInfo ProjectileSetting;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int32 CurrentPenetration = 1;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		virtual void InitProjectile(FProjectileInfo InitParam);

	virtual void ImpactProjectile();

	//Collision start
	//UFUNCTION()
	//	virtual void BulletCollisionSphereHit(class UPrimitiveComponent* HitComp, AActor* OtherActor,
	//		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		virtual void BulletCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//Collision end

	//Change velocity
	virtual void PostNetReceiveVelocity(const FVector& NewVelocity) override;
};
