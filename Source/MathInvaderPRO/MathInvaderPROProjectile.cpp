// Copyright 2024 Peteresi Games. All Rights Reserved.

#include "MathInvaderPROProjectile.h"
#include "MathInvaderPROCharacter.h"
#include "MathInvaderPROEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"

AMathInvaderPROProjectile::AMathInvaderPROProjectile()
	: ProjectileOwner(EProjectileOwner::Player)
	, Damage(25.0f)
	, ProjectileSpeed(800.0f)
	, MaxLifetime(5.0f)
	, AliveTime(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	CollisionCapsule->InitCapsuleSize(5.0f, 20.0f);
	CollisionCapsule->SetCollisionProfileName(TEXT("Projectile"));
	CollisionCapsule->OnComponentHit.AddDynamic(this, &AMathInvaderPROProjectile::OnHit);
	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMathInvaderPROProjectile::OnOverlap);
	RootComponent = CollisionCapsule;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailEffect"));
	TrailEffect->SetupAttachment(RootComponent);
	TrailEffect->bAutoActivate = true;

	// Default direction is forward (Y axis in 2D game setup)
	MoveDirection = FVector(0.0f, 1.0f, 0.0f);
}

void AMathInvaderPROProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AMathInvaderPROProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move in direction
	FVector NewLocation = GetActorLocation() + MoveDirection * ProjectileSpeed * DeltaTime;
	SetActorLocation(NewLocation, true);

	// Auto-destroy after max lifetime
	AliveTime += DeltaTime;
	if (AliveTime >= MaxLifetime)
	{
		Destroy();
	}
}

void AMathInvaderPROProjectile::InitProjectile(FVector Direction, float Speed)
{
	MoveDirection = Direction.GetSafeNormal();
	ProjectileSpeed = Speed;
}

void AMathInvaderPROProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner()) return;

	// Player projectile hitting an enemy
	if (ProjectileOwner == EProjectileOwner::Player)
	{
		if (AMathInvaderPROEnemy* Enemy = Cast<AMathInvaderPROEnemy>(OtherActor))
		{
			// Enemies are only defeated by correct answers, not direct hits
			// (This projectile is the visual feedback for answer submission)
		}
	}
	// Enemy projectile hitting the player
	else if (ProjectileOwner == EProjectileOwner::Enemy)
	{
		if (AMathInvaderPROCharacter* Player = Cast<AMathInvaderPROCharacter>(OtherActor))
		{
			Player->TakeDamageAmount(Damage);
		}
	}

	Destroy();
}

void AMathInvaderPROProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner()) return;

	if (ProjectileOwner == EProjectileOwner::Enemy)
	{
		if (AMathInvaderPROCharacter* Player = Cast<AMathInvaderPROCharacter>(OtherActor))
		{
			Player->TakeDamageAmount(Damage);
			Destroy();
		}
	}
}
