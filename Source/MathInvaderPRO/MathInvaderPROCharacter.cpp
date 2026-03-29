// Copyright 2024 Peteresi Games. All Rights Reserved.

#include "MathInvaderPROCharacter.h"
#include "MathInvaderPROProjectile.h"
#include "MathInvaderPROGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AMathInvaderPROCharacter::AMathInvaderPROCharacter()
	: MaxHealth(100.0f)
	, MoveSpeed(500.0f)
	, FireRate(0.5f)
	, MovementBoundsX(800.0f)
	, Health(100.0f)
	, bShieldActive(false)
	, bCanFire(true)
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(40.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = CollisionComponent;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(RootComponent);
	ShipMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetupAttachment(RootComponent);
	ShieldMesh->SetVisibility(false);
	ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
	ProjectileSpawnPoint->SetRelativeLocation(FVector(0.0f, 60.0f, 0.0f));
}

void AMathInvaderPROCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	bCanFire = true;
}

void AMathInvaderPROCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ClampPositionToBounds();
}

void AMathInvaderPROCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &AMathInvaderPROCharacter::MoveRight);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMathInvaderPROCharacter::FireProjectile);
}

void AMathInvaderPROCharacter::FireProjectile()
{
	if (!bCanFire || !ProjectileClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	AMathInvaderPROProjectile* Projectile = World->SpawnActor<AMathInvaderPROProjectile>(
		ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Projectile)
	{
		OnProjectileFired.Broadcast(Projectile);
	}

	// Apply fire cooldown
	bCanFire = false;
	GetWorldTimerManager().SetTimer(
		FireCooldownTimer,
		this,
		&AMathInvaderPROCharacter::OnFireCooldownComplete,
		FireRate,
		false
	);
}

void AMathInvaderPROCharacter::TakeDamageAmount(float DamageAmount)
{
	if (bShieldActive) return;

	Health = FMath::Max(0.0f, Health - DamageAmount);

	if (Health <= 0.0f)
	{
		OnPlayerDestroyed.Broadcast();

		AMathInvaderPROGameMode* GM = Cast<AMathInvaderPROGameMode>(
			UGameplayStatics::GetGameMode(this));
		if (GM)
		{
			GM->LoseLife();
		}

		// Destroy the actor
		Destroy();
	}
}

void AMathInvaderPROCharacter::SetShieldActive(bool bActive)
{
	bShieldActive = bActive;
	if (ShieldMesh)
	{
		ShieldMesh->SetVisibility(bActive);
	}
}

void AMathInvaderPROCharacter::OnFireCooldownComplete()
{
	bCanFire = true;
}

void AMathInvaderPROCharacter::MoveRight(float AxisValue)
{
	if (FMath::Abs(AxisValue) > 0.1f)
	{
		FVector DeltaLocation = FVector(AxisValue * MoveSpeed * GetWorld()->GetDeltaSeconds(), 0.0f, 0.0f);
		AddActorLocalOffset(DeltaLocation, true);
	}
}

void AMathInvaderPROCharacter::ClampPositionToBounds()
{
	FVector Location = GetActorLocation();
	Location.X = FMath::Clamp(Location.X, -MovementBoundsX, MovementBoundsX);
	SetActorLocation(Location);
}
