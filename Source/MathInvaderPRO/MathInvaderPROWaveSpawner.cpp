// Copyright 2024 Peteresi Games. All Rights Reserved.

#include "MathInvaderPROWaveSpawner.h"
#include "MathInvaderPROGameMode.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AMathInvaderPROWaveSpawner::AMathInvaderPROWaveSpawner()
	: FormationBoundaryX(700.0f)
	, StepDownInterval(1.0f)
	, FormationMoveDirection(1.0f)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMathInvaderPROWaveSpawner::BeginPlay()
{
	Super::BeginPlay();

	// Listen for wave start events
	if (AMathInvaderPROGameMode* GM = Cast<AMathInvaderPROGameMode>(
		UGameplayStatics::GetGameMode(this)))
	{
		GM->OnWaveStarted.AddDynamic(this, &AMathInvaderPROWaveSpawner::SpawnWave);
	}
}

void AMathInvaderPROWaveSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateFormationMovement(DeltaTime);
	CheckBoundaryCollision();

	// Check if all enemies are defeated
	if (ActiveEnemies.Num() == 0) return;

	int32 AliveCount = GetAliveEnemyCount();
	if (AliveCount == 0)
	{
		ActiveEnemies.Empty();
		GetWorldTimerManager().ClearTimer(StepDownTimerHandle);

		if (AMathInvaderPROGameMode* GM = Cast<AMathInvaderPROGameMode>(
			UGameplayStatics::GetGameMode(this)))
		{
			GM->OnWaveCleared();
		}
	}
}

void AMathInvaderPROWaveSpawner::SpawnWave(int32 WaveIndex)
{
	ClearAllEnemies();

	// Use wave config if available, otherwise use a default single-row config
	int32 ConfigIndex = FMath::Clamp(WaveIndex - 1, 0, WaveConfigs.Num() - 1);

	if (WaveConfigs.Num() == 0 || ConfigIndex < 0)
	{
		// Fallback: spawn a basic row of 8 enemies
		if (!DefaultEnemyClass) return;

		for (int32 Col = 0; Col < 8; Col++)
		{
			FVector SpawnLocation = FVector(
				(Col - 3.5f) * 100.0f,
				600.0f,
				0.0f
			);

			FActorSpawnParameters Params;
			AMathInvaderPROEnemy* Enemy = GetWorld()->SpawnActor<AMathInvaderPROEnemy>(
				DefaultEnemyClass, SpawnLocation, FRotator::ZeroRotator, Params);

			if (Enemy)
			{
				Enemy->SetFormationIndex(0, Col);
				Enemy->SetMovementDirection(FormationMoveDirection);
				Enemy->OnEnemyDestroyed.AddDynamic(this, &AMathInvaderPROWaveSpawner::OnEnemyDestroyed);
				ActiveEnemies.Add(Enemy);
			}
		}
	}
	else
	{
		const FWaveConfig& Config = WaveConfigs[ConfigIndex];

		for (int32 Row = 0; Row < Config.Rows.Num(); Row++)
		{
			const FEnemyFormationRow& RowConfig = Config.Rows[Row];
			TSubclassOf<AMathInvaderPROEnemy> ClassToSpawn =
				RowConfig.EnemyClass ? RowConfig.EnemyClass : DefaultEnemyClass;

			if (!ClassToSpawn) continue;

			float RowY = Config.StartY - (Row * Config.RowSpacing);
			float TotalWidth = (RowConfig.Count - 1) * RowConfig.Spacing;

			for (int32 Col = 0; Col < RowConfig.Count; Col++)
			{
				float X = -TotalWidth / 2.0f + Col * RowConfig.Spacing;
				FVector SpawnLocation = FVector(X, RowY, 0.0f);

				FActorSpawnParameters Params;
				AMathInvaderPROEnemy* Enemy = GetWorld()->SpawnActor<AMathInvaderPROEnemy>(
					ClassToSpawn, SpawnLocation, FRotator::ZeroRotator, Params);

				if (Enemy)
				{
					float SpeedFactor = Config.SpeedMultiplier;
					Enemy->HorizontalSpeed *= SpeedFactor;
					Enemy->SetFormationIndex(Row, Col);
					Enemy->SetMovementDirection(FormationMoveDirection);
					Enemy->OnEnemyDestroyed.AddDynamic(this, &AMathInvaderPROWaveSpawner::OnEnemyDestroyed);
					ActiveEnemies.Add(Enemy);
				}
			}
		}
	}

	// Start the step-down timer
	GetWorldTimerManager().SetTimer(
		StepDownTimerHandle,
		this,
		&AMathInvaderPROWaveSpawner::OnStepDownTimer,
		StepDownInterval,
		true
	);
}

void AMathInvaderPROWaveSpawner::ClearAllEnemies()
{
	GetWorldTimerManager().ClearTimer(StepDownTimerHandle);

	for (AMathInvaderPROEnemy* Enemy : ActiveEnemies)
	{
		if (IsValid(Enemy))
		{
			Enemy->Destroy();
		}
	}
	ActiveEnemies.Empty();
}

int32 AMathInvaderPROWaveSpawner::GetAliveEnemyCount() const
{
	int32 Count = 0;
	for (const AMathInvaderPROEnemy* Enemy : ActiveEnemies)
	{
		if (IsValid(Enemy))
		{
			Count++;
		}
	}
	return Count;
}

void AMathInvaderPROWaveSpawner::UpdateFormationMovement(float DeltaTime)
{
	// Formation movement is handled per-enemy in their own Tick
	// The spawner checks boundaries and triggers step-down
}

void AMathInvaderPROWaveSpawner::OnStepDownTimer()
{
	// Check if any enemy has hit the horizontal boundary
	bool bHitBoundary = false;
	for (AMathInvaderPROEnemy* Enemy : ActiveEnemies)
	{
		if (IsValid(Enemy))
		{
			float EnemyX = Enemy->GetActorLocation().X;
			if (FMath::Abs(EnemyX) >= FormationBoundaryX)
			{
				bHitBoundary = true;
				break;
			}
		}
	}

	if (bHitBoundary)
	{
		FormationMoveDirection *= -1.0f;
		for (AMathInvaderPROEnemy* Enemy : ActiveEnemies)
		{
			if (IsValid(Enemy))
			{
				Enemy->StepDown();
				Enemy->SetMovementDirection(FormationMoveDirection);
			}
		}
	}
}

void AMathInvaderPROWaveSpawner::CheckBoundaryCollision()
{
	// Boundary collision is checked per-tick in OnStepDownTimer
}

void AMathInvaderPROWaveSpawner::OnEnemyDestroyed(AMathInvaderPROEnemy* DestroyedEnemy)
{
	ActiveEnemies.Remove(DestroyedEnemy);
}
