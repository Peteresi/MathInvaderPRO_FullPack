// Copyright 2024 Peteresi Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MathQuestionManager.h"
#include "MathInvaderPROGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveStarted, int32, WaveNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveCompleted, int32, WaveNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVictory);

/**
 * Game Mode for MathInvaderPRO.
 * Manages waves of enemies, scoring, lives, and progression.
 */
UCLASS()
class MATHINVADERPRO_API AMathInvaderPROGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMathInvaderPROGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// ---- Wave Management ----

	/** Start the next wave of invaders */
	UFUNCTION(BlueprintCallable, Category = "Game|Waves")
	void StartNextWave();

	/** Called when all enemies in the current wave are defeated */
	UFUNCTION(BlueprintCallable, Category = "Game|Waves")
	void OnWaveCleared();

	/** Get the current wave number */
	UFUNCTION(BlueprintPure, Category = "Game|Waves")
	int32 GetCurrentWave() const { return CurrentWave; }

	/** Get the total number of waves */
	UFUNCTION(BlueprintPure, Category = "Game|Waves")
	int32 GetTotalWaves() const { return TotalWaves; }

	// ---- Score & Lives ----

	/** Add points to the player's score */
	UFUNCTION(BlueprintCallable, Category = "Game|Score")
	void AddScore(int32 Points);

	/** Get the current score */
	UFUNCTION(BlueprintPure, Category = "Game|Score")
	int32 GetScore() const { return CurrentScore; }

	/** Get the current number of lives */
	UFUNCTION(BlueprintPure, Category = "Game|Lives")
	int32 GetLives() const { return CurrentLives; }

	/** Deduct a life; triggers game over if lives reach 0 */
	UFUNCTION(BlueprintCallable, Category = "Game|Lives")
	void LoseLife();

	// ---- Math Question Integration ----

	/** Get the math question manager */
	UFUNCTION(BlueprintPure, Category = "Game|Math")
	UMathQuestionManager* GetMathQuestionManager() const { return MathQuestionManager; }

	/** Called when the player answers a math question correctly */
	UFUNCTION(BlueprintCallable, Category = "Game|Math")
	void OnCorrectAnswer(int32 EnemyValue);

	/** Called when the player answers a math question incorrectly */
	UFUNCTION(BlueprintCallable, Category = "Game|Math")
	void OnWrongAnswer();

	// ---- Game State Transitions ----

	/** Trigger game over */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void TriggerGameOver();

	/** Trigger victory */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void TriggerVictory();

	// ---- Events ----

	UPROPERTY(BlueprintAssignable, Category = "Game|Events")
	FOnWaveStarted OnWaveStarted;

	UPROPERTY(BlueprintAssignable, Category = "Game|Events")
	FOnWaveCompleted OnWaveCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Game|Events")
	FOnGameOver OnGameOver;

	UPROPERTY(BlueprintAssignable, Category = "Game|Events")
	FOnVictory OnVictory;

	// ---- Settings ----

	/** Total number of waves before victory */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Settings")
	int32 TotalWaves;

	/** Starting number of lives */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Settings")
	int32 StartingLives;

	/** Score awarded for each correct answer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Settings")
	int32 BaseScorePerKill;

	/** Score multiplier for combo hits (answering correctly multiple times in a row) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Settings")
	float ComboMultiplier;

	/** Difficulty level - used to configure the math question manager */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Settings")
	EDifficultyLevel DifficultyLevel;

	/** Math operation type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Settings")
	EMathOperationType OperationType;

	/** Time delay in seconds between waves */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Settings")
	float WaveDelaySeconds;

	/** Name of the level to load for game over */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Maps")
	FName GameOverLevelName;

	/** Name of the level to load for victory */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Maps")
	FName VictoryLevelName;

protected:
	UPROPERTY()
	UMathQuestionManager* MathQuestionManager;

	int32 CurrentWave;
	int32 CurrentScore;
	int32 CurrentLives;
	int32 ConsecutiveCorrect;

	FTimerHandle WaveStartTimerHandle;
};
