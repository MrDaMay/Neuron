// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/NR_EnemyCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/NR_PlayerState.h"
#include "Kismet/KismetMathLibrary.h"

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
}

float ANR_EnemyCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	GetHealthComponent()->ChangeHealthValue(DamageAmount * CoefDamageResist * -1);

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

	PlayAnimMontage(DeadMontage);

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

}

void ANR_EnemyCharacterBase::AttackEnemy(FName NotifyName,
	const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == "Attack")
	{
		TArray<AActor*> ActorIgnore;
		FVector StartTraceVector = GetActorLocation() + GetActorForwardVector() * 50.0f;
		FVector EndTraceVector = GetActorLocation() + GetActorForwardVector() * 200.0f;
		FHitResult Hit;
		UKismetSystemLibrary::SphereTraceSingle(this, StartTraceVector, EndTraceVector, RadiusMeleeAttack,
			ETraceTypeQuery::TraceTypeQuery14, false, ActorIgnore, EDrawDebugTrace::ForDuration,
			Hit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);

		if (Hit.HitObjectHandle.FetchActor())
		{
			if (Hit.HitObjectHandle.FetchActor()->ActorHasTag("Player"))
			{
				const TSubclassOf<UDamageType> DamageType;
				const float Damage = UKismetMathLibrary::RandomFloatInRange(BaseAttackDamage - RandomRangeAttackDamage, BaseAttackDamage + RandomRangeAttackDamage);

				UGameplayStatics::ApplyDamage(Hit.GetActor(), Damage, GetController(), this, DamageType);
			}
		}
	}
}

void ANR_EnemyCharacterBase::CharacterDead_BP_Implementation()
{
}

void ANR_EnemyCharacterBase::HealthChange_BP_Implementation(float Health, float Damage)
{
}

