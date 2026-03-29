// Copyright 2024 Peteresi Games. All Rights Reserved.

#include "MathInvaderPROEnemy.h"
#include "MathInvaderPROProjectile.h"
#include "MathInvaderPROGameMode.h"
#include "MathInvaderPROGameState.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AMathInvaderPROEnemy::AMathInvaderPROEnemy()
	: EnemyType(EEnemyType::Basic)
	, PointValue(1)
	, HorizontalSpeed(100.0f)
	, VerticalStepSize(30.0f)
	, FireInterval(3.0f)
	, BoundaryX(700.0f)
	, BottomBoundaryY(-400.0f)
	, MoveDirection(1.0f)
	, FormationRow(0)
	, FormationColumn(0)
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetBoxExtent(FVector(40.0f, 40.0f, 40.0f));
	CollisionBox->SetCollisionProfileName(TEXT("BlockAll"));
	RootComponent = CollisionBox;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetupAttachment(RootComponent);
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AnswerText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("AnswerText"));
	AnswerText->SetupAttachment(RootComponent);
	AnswerText->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	AnswerText->SetHorizontalAlignment(EHTA_Center);
	AnswerText->SetWorldSize(48.0f);
	AnswerText->SetText(FText::FromString(TEXT("?")));

	// Set point values based on enemy type defaults
	switch (EnemyType)
	{
		case EEnemyType::Fast:   PointValue = 2; HorizontalSpeed = 160.0f; break;
		case EEnemyType::Tank:   PointValue = 3; HorizontalSpeed = 60.0f;  break;
		case EEnemyType::Boss:   PointValue = 10; HorizontalSpeed = 80.0f; break;
		default:                 PointValue = 1; HorizontalSpeed = 100.0f; break;
	}
}

void AMathInvaderPROEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Start fire timer with a random initial offset to stagger enemy fire
	float InitialDelay = FMath::FRandRange(0.5f, FireInterval);
	GetWorldTimerManager().SetTimer(
		FireTimerHandle,
		this,
		&AMathInvaderPROEnemy::OnFireTimerExpired,
		FireInterval,
		true,
		InitialDelay
	);
}

void AMathInvaderPROEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move horizontally
	FVector Location = GetActorLocation();
	Location.X += MoveDirection * HorizontalSpeed * DeltaTime;
	SetActorLocation(Location);

	// Check if reached the bottom boundary
	if (Location.Y <= BottomBoundaryY)
	{
		OnEnemyReachedBottom.Broadcast();

		// Trigger game over through game mode
		AMathInvaderPROGameMode* GM = Cast<AMathInvaderPROGameMode>(
			UGameplayStatics::GetGameMode(this));
		if (GM)
		{
			GM->TriggerGameOver();
		}
	}
}

void AMathInvaderPROEnemy::OnCorrectAnswerReceived()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
	OnEnemyDestroyed.Broadcast(this);

	// Increment enemies defeated in game state
	if (AMathInvaderPROGameState* GS = Cast<AMathInvaderPROGameState>(
		UGameplayStatics::GetGameState(this)))
	{
		GS->IncrementEnemiesDefeated();
	}

	Destroy();
}

void AMathInvaderPROEnemy::FireAtPlayer()
{
	if (!ProjectileClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	FVector SpawnLocation = GetActorLocation() + FVector(0.0f, -60.0f, 0.0f);
	FRotator SpawnRotation = FRotator(0.0f, -90.0f, 0.0f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	World->SpawnActor<AMathInvaderPROProjectile>(
		ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
}

void AMathInvaderPROEnemy::SetFormationIndex(int32 Row, int32 Column)
{
	FormationRow = Row;
	FormationColumn = Column;
}

void AMathInvaderPROEnemy::StepDown()
{
	FVector Location = GetActorLocation();
	Location.Y -= VerticalStepSize;
	SetActorLocation(Location);
	MoveDirection *= -1.0f;
}

void AMathInvaderPROEnemy::OnFireTimerExpired()
{
	// Only fire if the game is active and there's a random chance
	if (FMath::FRand() < 0.3f)
	{
		FireAtPlayer();
	}
}
