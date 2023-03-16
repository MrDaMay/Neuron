// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NR_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Game/NR_GameInstance.h"
#include "Game/NR_GameState.h"
#include "Game/NR_PlayerState.h"
#include "Game/NR_PlayerController.h"
#include "Enemy/NR_FreezeInterface.h"

// Sets default values
ANR_Character::ANR_Character()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	HealthComponent = CreateDefaultSubobject<UNR_HealthComponent>(TEXT("CharHealthComponent"));
	if (HealthComponent)
	{
		HealthComponent->OnDead.AddDynamic(this, &ANR_Character::CharDead);
	}

	TokenComponent = CreateDefaultSubobject<UNR_TokenComponent>(TEXT("CharTokenComponent"));
	if (TokenComponent)
	{
		TokenComponent->InitTokens();
	}
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

}

// Called when the game starts or when spawned
void ANR_Character::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld() && GetWorld()->GetNetMode() != NM_DedicatedServer)
	{
		if (CursorMaterial && (GetLocalRole() == ROLE_AutonomousProxy || GetLocalRole() == ROLE_Authority))
		{
			CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
		}
	}

	//Initialize character stats from GameInstance
	auto GameInstance = Cast<UNR_GameInstance>(GetGameInstance());
	if (GameInstance)
	{
		Stats = GameInstance->GetInitialStats();
	}

	//Sending initial stats to GameState for managing any changes and bind to Stats Update
	auto GameState = Cast<ANR_GameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		GameState->SetStats(Stats);
		GameState->OnCharStatsChanged.AddDynamic(this, &ANR_Character::UpdateStats);
		GameState->StartWavePhase();
	}

	InitWeapon("Rifle");

	auto PlayerController = Cast<ANR_PlayerController>(GetController());
	PlayerController->OnEndGame.AddDynamic(this, &ANR_Character::AbsolutelyDead);
}

float ANR_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(!Stats.Immortality)
		HealthComponent->ChangeHealthValue(-DamageAmount);

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

// Called every frame
void ANR_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentCursor)
	{
		ANR_PlayerController* PlayerController = Cast<ANR_PlayerController>(GetController());

		FHitResult TraceHitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
		FVector CursorLocation = TraceHitResult.ImpactNormal;
		FRotator CursorRotation = CursorLocation.Rotation();

		CurrentCursor->SetWorldLocation(TraceHitResult.Location);
		CurrentCursor->SetWorldRotation(CursorRotation);
	}

	MovementTick(DeltaTime);
}

// Called to bind functionality to input
void ANR_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ANR_Character::InputAxisX);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ANR_Character::InputAxisY);

	PlayerInputComponent->BindAction(TEXT("FireEvent"), EInputEvent::IE_Pressed, this, &ANR_Character::InputAttackPressed);
	PlayerInputComponent->BindAction(TEXT("FireEvent"), EInputEvent::IE_Released, this, &ANR_Character::InputAttackReleased);

	PlayerInputComponent->BindAction(TEXT("RollEvent"), EInputEvent::IE_Pressed, this, &ANR_Character::InputRollPressed);
}

void ANR_Character::InputAxisX(float Value)
{
	AxisX = Value;
}

void ANR_Character::InputAxisY(float Value)
{
	AxisY = Value;
}

void ANR_Character::InputAttackPressed()
{
	if (HealthComponent && HealthComponent->GetIsAlive() && !RollEnable)
	{
		PlayAnimMontage(CurrentWeapon->WeaponSetting.AnimWeaponInfo.CharacterFireAnimMontage);
		AttackEvent(true);
	}
}

void ANR_Character::InputAttackReleased()
{
	AttackEvent(false);
}

void ANR_Character::InputRollPressed()
{
	if (!RollEnable)
	{
		RollEnable = true;
		PlayAnimMontage(RollMontage);
		Stats.Immortality = true;

		GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed * 2;

		GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel5);
	}
}

ANR_Weapon* ANR_Character::GetCurrentWeapon()
{
	return CurrentWeapon;
}

void ANR_Character::InitWeapon(FName IdWeaponName)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	UNR_GameInstance* GameInstance = Cast<UNR_GameInstance>(GetGameInstance());
	FWeaponInfo WeaponInfo;

	if (GameInstance)
	{
		if (GameInstance->GetWeaponInfoByName(IdWeaponName, WeaponInfo))
		{
			if (WeaponInfo.WeaponClass)
			{
				FVector SpawnLocation = FVector(0);
				FRotator SpawnRotation = FRotator(0);

				FActorSpawnParameters SpawmParams;
				SpawmParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawmParams.Owner = this;
				SpawmParams.Instigator = GetInstigator();

				ANR_Weapon* Weapon = Cast<ANR_Weapon>(GetWorld()->SpawnActor(WeaponInfo.WeaponClass, &SpawnLocation,
					&SpawnRotation, SpawmParams));

				if (Weapon)
				{
					FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
					Weapon->AttachToComponent(GetMesh(), Rule, FName("WeaponSocketRightHand"));
					CurrentWeapon = Weapon;

					Weapon->Pawn = this;
					Weapon->WeaponSetting = WeaponInfo;

					Weapon->OnWeaponFireStart.AddDynamic(this, &ANR_Character::WeaponFireStart);
					OnWeaponSwitched.Broadcast(IdWeaponName);
				}
			}
		}
	}
}


void ANR_Character::WeaponFireStart(UAnimMontage* Anim)
{
	PlayAnimMontage(Anim, 1.0f, "Mesh");

	WeaponFireStart_BP(Anim);
}

void ANR_Character::StartSwitchWeapon()
{
	//OverlapPickUpWeapon = Weapon;

	StartSwitchWeapon_BP();
}

void ANR_Character::EndSwitchWeapon()
{
	//OverlapPickUpWeapon = nullptr;

	EndSwitchWeapon_BP();
}

void ANR_Character::MovementTick(float DeltaTime)
{
	ANR_PlayerController* PlayerController = Cast<ANR_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	PlayerController->DeprojectMousePositionToWorld(MouseLocation, LineEnd);

	LineEnd = (LineEnd * 5000) + MouseLocation;
	const FVector PointUnderCursor = FMath::LinePlaneIntersection(MouseLocation, LineEnd, GetActorLocation(), FVector(0, 0, 1));

	const float RotatingToCursor = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PointUnderCursor).Yaw;

	if (!RollEnable)
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisX);
		AddMovementInput(FVector(0.0f, 1.0f, 0.0f), AxisY);

		SetActorRotation(FQuat(FRotator(0.0f, RotatingToCursor, 0.0f)));
	}
	else
	{
		if (RollTime < MaxRollTime)
		{
			AddMovementInput(GetActorForwardVector());
			RollTime = RollTime + DeltaTime;
		}
		else
		{
			RollEnable = false;
			RollTime = 0.0f;
			Stats.Immortality = false;

			GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed / 2;

			GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
		}
	}
}

void ANR_Character::AttackEvent(bool bIsFire)
{
	if (GetCurrentWeapon())
	{
		GetCurrentWeapon()->FireButtonPressed(bIsFire);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("ASnowBall_TPSCharacter::AttackCharEvent - CurrentWeapon - NULL"));
}

void ANR_Character::CharDead()
{
	auto PlayerController = Cast<ANR_PlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->TryToRespawnPlayer();
	}
}

void ANR_Character::AbsolutelyDead(bool IsWin)
{
	PlayAnimMontage(DeathMontage);
}

void ANR_Character::TakeBonus(FName BonusType)
{
	auto MyPlayerState = Cast<ANR_PlayerState>(GetPlayerState());
	if (MyPlayerState)
	{
		MyPlayerState->IncrementNumBonus();
	}

	if (BonusType == "Aid")
	{
		HealthComponent->ChangeHealthValue(50.0f);
	}
	else
	{
		if (BonusType == "Fire")
		{
			//Change params
			Stats.CoefFireSpeed = Stats.CoefFireSpeed * 2;
			Stats.CoefDamage = Stats.CoefDamage * 2;

			//Say weapon about changes
			OnWeaponParamsChange.Broadcast(Stats.CoefFireSpeed, Stats.CoefDamage);

			//Clean the timer before use
			if (GetWorldTimerManager().IsTimerActive(FinishFireBonusTimerHamdle))
				GetWorldTimerManager().ClearTimer(FinishFireBonusTimerHamdle);

			//Set timer
			GetWorldTimerManager().SetTimer(FinishFireBonusTimerHamdle, this, &ANR_Character::FinishFireBonus, 15.0f, false, 15.0f);
		}
		else
		{
			if (BonusType == "Speed")
			{
				//Change params
				Stats.CoefMovementSpeed = Stats.CoefMovementSpeed * 2;
				GetCharacterMovement()->MaxWalkSpeed = Stats.BaseSpeed * Stats.CoefMovementSpeed;

				//Clean the timer before use
				if (GetWorldTimerManager().IsTimerActive(FinishMovementSpeedBonusTimerHamdle))
					GetWorldTimerManager().ClearTimer(FinishMovementSpeedBonusTimerHamdle);

				//Set timer
				GetWorldTimerManager().SetTimer(FinishMovementSpeedBonusTimerHamdle, this, &ANR_Character::FinishMovementSpeedBonus, 10.0f, false, 10.0f);
			}
			else
			{
				if (BonusType == "Immortality")
				{
					//Change params
					Stats.Immortality = true;

					//Clean the timer before use
					if (GetWorldTimerManager().IsTimerActive(FinishImmortalityBonusTimerHamdle))
						GetWorldTimerManager().ClearTimer(FinishImmortalityBonusTimerHamdle);

					//Set timer
					GetWorldTimerManager().SetTimer(FinishImmortalityBonusTimerHamdle, this, &ANR_Character::FinishImmortalityBonus, 5.0f, false, 5.0f);

				}
				else
				{
					if (BonusType == "Freeze")
					{
						//Freeze function
						FreezeBonusFunction();
					}
				}
			}
		}
	}
}

void ANR_Character::FinishFireBonus()
{
	//Reset params
	Stats.CoefFireSpeed = Stats.CoefFireSpeed / 2;
	Stats.CoefDamage = Stats.CoefDamage / 2;

	//Say weapon about changes
	OnWeaponParamsChange.Broadcast(Stats.CoefFireSpeed, Stats.CoefDamage);
}

void ANR_Character::FinishImmortalityBonus()
{
	//Reset params
	Stats.Immortality = false;
}

void ANR_Character::FinishMovementSpeedBonus()
{
	//Reset params
	Stats.CoefMovementSpeed = Stats.CoefMovementSpeed / 2;
	GetCharacterMovement()->MaxWalkSpeed = Stats.BaseSpeed * Stats.CoefMovementSpeed;
}

void ANR_Character::FreezeBonusFunction()
{
	TArray<FHitResult> Hit;
	TArray<AActor*> Actors;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(),
		1500.0f, ETraceTypeQuery::TraceTypeQuery3, false, Actors,
		EDrawDebugTrace::ForDuration, Hit, true, FLinearColor::Green, 
		FLinearColor::Red, 5.0f);

	for (int i = 0; i<Hit.Num(); i++)
	{
		INR_FreezeInterface* FreezeInterface = Cast<INR_FreezeInterface>(Hit[i].GetActor());
		if(FreezeInterface)
			FreezeInterface->Freeze();
	}
}

void ANR_Character::AddNewWeaponToWeaponSlot(FName NewWeapon)
{
	WeaponSLot.Add(NewWeapon);
}

TArray<FName> ANR_Character::GetWeaponSlot()
{
	return WeaponSLot;
}

void ANR_Character::TakeToken(FName Token)
{
	if (Token == "MoveSpeed")
	{
		TokenComponent->CollectToken(0);
	}
	if (Token == "Strength")
	{
		TokenComponent->CollectToken(1);
	}
	if (Token == "Health")
	{
		TokenComponent->CollectToken(2);
	}
	if (Token == "FireSpeed")
	{
		TokenComponent->CollectToken(3);
	}
	if (Token == "Power")
	{
		TokenComponent->CollectToken(4);
	}
	if (Token == "Luck")
	{
		TokenComponent->CollectToken(5);
	}
}

void ANR_Character::UpdateStats(FCharStats NewStats)
{
	Stats = NewStats;
	HealthComponent->CoefDamageResist = Stats.CoefDamageResist;
}

void ANR_Character::CharDead_BP_Implementation()
{
}

void ANR_Character::EndSwitchWeapon_BP_Implementation()
{
}

void ANR_Character::StartSwitchWeapon_BP_Implementation()
{
}

void ANR_Character::WeaponFireStart_BP_Implementation(UAnimMontage* Anim)
{
}

