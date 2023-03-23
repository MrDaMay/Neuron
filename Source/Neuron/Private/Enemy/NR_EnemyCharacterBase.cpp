// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/NR_EnemyCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/NR_PlayerState.h"
#include "Game/NR_GameInstance.h"
#include "Collectable/NR_CollectableBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ANR_EnemyCharacterBase::ANR_EnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	HealthComponent = CreateDefaultSubobject<UNR_HealthComponent>(TEXT("HealthComponent"));
	if (HealthComponent)
	{
		HealthComponent->OnDead.AddDynamic(this, &ANR_EnemyCharacterBase::CharacterDead);
		HealthComponent->OnHealthChange.AddDynamic(this, &ANR_EnemyCharacterBase::HealthChange);
	}

	KillScoreComponent = CreateDefaultSubobject<UNR_KillScoreComponent>(TEXT("KillScoreComponent"));
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->CustomDepthStencilValue = 1;

	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
}

// Called when the game starts or when spawned
void ANR_EnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (AttackMontage)
	{
		AnimAttackInstance = GetMesh()->GetAnimInstance();

		if (AnimAttackInstance)
		{
			AnimAttackInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ANR_EnemyCharacterBase::AttackEnemy);
			AnimAttackInstance->OnMontageEnded.AddDynamic(this, &ANR_EnemyCharacterBase::AttackAnimEnd);
		}
	}

	if(SpawnSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpawnSound, GetActorLocation());
}

float ANR_EnemyCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	GetHealthComponent()->ChangeHealthValue(-DamageAmount);

	if (HitSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

// Called every frame
void ANR_EnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANR_EnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ANR_EnemyCharacterBase::CanEnemyAttack()
{
	return CanAttackPawn;
}

bool ANR_EnemyCharacterBase::CanEnemyMove()
{
	return CanMovePawn;
}

void ANR_EnemyCharacterBase::StartAttackEnemy()
{
	SetStateAttack(false);

	if (AttackMontage)
	{
		if (AnimAttackInstance)
		{
			AnimAttackInstance->Montage_Play(AttackMontage);
		}
	}
}

void ANR_EnemyCharacterBase::HealthChange(float Health, float Damage)
{
	HealthChange_BP(Health, Damage);
}

void ANR_EnemyCharacterBase::CharacterDead()
{
	KillScoreComponent->OwnerWasDead();

	GetCapsuleComponent()->SetCollisionProfileName("DeadEnemy");

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
	}
	if (IsValid(GetController()))
	{
		GetController()->UnPossess();
	}

	SetLifeSpan(2.0f);
	SetStateMove(false);
	SetStateAttack(false);

	Drop();

	PlayAnimMontage(DeadMontage);
	if(DeathSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());

	CharacterDead_BP();
}

void ANR_EnemyCharacterBase::SetStateAttack(bool CanAttack)
{
	CanAttackPawn = CanAttack;
}

void ANR_EnemyCharacterBase::SetStateMove(bool CanMove)
{
	CanMovePawn = CanMove;
}

void ANR_EnemyCharacterBase::AttackAnimEnd(UAnimMontage* Montage, bool bIsSuccess)
{
	SetStateAttack(true);
}

void ANR_EnemyCharacterBase::AttackEnemy(FName NotifyName,
	const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == "Attack")
	{
		TArray<AActor*> ActorIgnore;
		FVector StartTraceVector = GetActorLocation() + GetActorForwardVector() * 50.0f;
		FVector EndTraceVector = GetActorLocation() + GetActorForwardVector() * 250.0f;
		FHitResult Hit;
		UKismetSystemLibrary::SphereTraceSingle(this, StartTraceVector, EndTraceVector, RadiusMeleeAttack,
			ETraceTypeQuery::TraceTypeQuery14, false, ActorIgnore, EDrawDebugTrace::ForDuration,
			Hit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);

		if (Hit.HitObjectHandle.FetchActor())
		{
			if (Hit.HitObjectHandle.FetchActor()->ActorHasTag("Player"))
			{
				const TSubclassOf<UDamageType> DamageType;
				UGameplayStatics::ApplyDamage(Hit.GetActor(), BaseAttackDamage, GetController(), this, DamageType);
			}
		}
	}
}

void ANR_EnemyCharacterBase::Freeze(float FreezeTime)
{
	SetStateMove(false);
	GetWorldTimerManager().SetTimer(FreezeTimerHandle, this, &ANR_EnemyCharacterBase::EndFreeze, FreezeTime, false, FreezeTime);
}

void ANR_EnemyCharacterBase::EndFreeze()
{
	SetStateMove(true);
}

void ANR_EnemyCharacterBase::Drop()
{
	if (DropChance >= UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f))
	{
		FDropObjects SpawnObject;
		int32 DropObjectIndex = UKismetMathLibrary::RandomIntegerInRange(0, DropObjectNames.Num() - 1);

		FVector SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 94);
		FRotator SpawnRotator = FRotator(0);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		auto GameInstance = Cast<UNR_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		GameInstance->GetDropInfoByName(DropObjectNames[DropObjectIndex], SpawnObject);

		auto SpawnActor = Cast<ANR_CollectableBase>(GetWorld()->SpawnActor(SpawnObject.DropItem, &SpawnLocation, &SpawnRotator, SpawnParams));
		if (SpawnActor)
		{
			SpawnActor->StaticMesh->SetStaticMesh(SpawnObject.DropMesh);
			SpawnActor->LightPartical->Template = SpawnObject.LightPartical;
			SpawnActor->OverlapSpawnPartical = SpawnObject.OverlapPartical;
			SpawnActor->ObjectName = DropObjectNames[DropObjectIndex];
			SpawnActor->PickUpSound = SpawnObject.PickUpSound;
		}
	}
}

void ANR_EnemyCharacterBase::CharacterDead_BP_Implementation()
{
}

void ANR_EnemyCharacterBase::HealthChange_BP_Implementation(float Health, float Damage)
{
}

