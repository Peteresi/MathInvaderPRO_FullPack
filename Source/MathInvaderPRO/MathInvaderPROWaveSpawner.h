// Copyright 2024 Peteresi Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MathInvaderPROEnemy.h"
#include "MathInvaderPROWaveSpawner.generated.h"

USTRUCT(BlueprintType)
struct FEnemyFormationRow
{
	GENERATED_BODY()

	/** The enemy class to spawn for this row */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formation")
	TSubclassOf<AMathInvaderPROEnemy> EnemyClass;

	/** Number of enemies in this row */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formation", meta = (ClampMin = "1", ClampMax = "12"))
	int32 Count;

	/** Horizontal spacing between enemies */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formation")
	float Spacing;

	FEnemyFormationRow()
		: EnemyClass(nullptr)
		, Count(8)
		, Spacing(100.0f)
	{}
};

USTRUCT(BlueprintType)
struct FWaveConfig
{
	GENERATED_BODY()

	/** Rows of enemies in this wave */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	TArray<FEnemyFormationRow> Rows;

	/** Horizontal movement speed multiplier for this wave */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float SpeedMultiplier;

	/** Row Y start position (top of screen) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float StartY;

	/** Vertical spacing between rows */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float RowSpacing;

	FWaveConfig()
		: SpeedMultiplier(1.0f)
		, StartY(600.0f)
		, RowSpacing(80.0f)
	{}
};

/**
 * Wave spawner actor for MathInvaderPRO.
 * Manages spawning and coordinating enemy formations per wave.
 */
UCLASS()
class MATHINVADERPRO_API AMathInvaderPROWaveSpawner : public AActor
{
	GENERATED_BODY()

public:
	AMathInvaderPROWaveSpawner();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Spawn the enemy formation for a given wave index */
	UFUNCTION(BlueprintCallable, Category = "Wave Spawner")
	void SpawnWave(int32 WaveIndex);

	/** Clear all existing enemies */
	UFUNCTION(BlueprintCallable, Category = "Wave Spawner")
	void ClearAllEnemies();

	/** Get the count of remaining alive enemies */
	UFUNCTION(BlueprintPure, Category = "Wave Spawner")
	int32 GetAliveEnemyCount() const;

	/** Get all currently active enemies */
	UFUNCTION(BlueprintPure, Category = "Wave Spawner")
	const TArray<AMathInvaderPROEnemy*>& GetActiveEnemies() const { return ActiveEnemies; }

	/** Wave configuration list - one entry per wave */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Spawner|Settings")
	TArray<FWaveConfig> WaveConfigs;

	/** Default enemy class if not specified per row */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Spawner|Settings")
	TSubclassOf<AMathInvaderPROEnemy> DefaultEnemyClass;

	/** Boundary X at which enemy formation reverses direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Spawner|Settings")
	float FormationBoundaryX;

	/** How often (in seconds) the formation steps down */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Spawner|Settings")
	float StepDownInterval;

protected:
	UPROPERTY()
	TArray<AMathInvaderPROEnemy*> ActiveEnemies;

	float FormationMoveDirection;
	FTimerHandle StepDownTimerHandle;

	void UpdateFormationMovement(float DeltaTime);
	void OnStepDownTimer();
	void CheckBoundaryCollision();

	UFUNCTION()
	void OnEnemyDestroyed(AMathInvaderPROEnemy* DestroyedEnemy);
};
