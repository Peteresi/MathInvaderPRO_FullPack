// Copyright 2024 Peteresi Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MathInvaderPROGameState.generated.h"

/**
 * Game State for MathInvaderPRO.
 * Holds replicated game data: score, lives, wave information.
 */
UCLASS()
class MATHINVADERPRO_API AMathInvaderPROGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMathInvaderPROGameState();

	/** Get the current score */
	UFUNCTION(BlueprintPure, Category = "Game State")
	int32 GetScore() const { return Score; }

	/** Set the current score */
	UFUNCTION(BlueprintCallable, Category = "Game State")
	void SetScore(int32 NewScore);

	/** Get the number of lives remaining */
	UFUNCTION(BlueprintPure, Category = "Game State")
	int32 GetLives() const { return Lives; }

	/** Set the number of lives remaining */
	UFUNCTION(BlueprintCallable, Category = "Game State")
	void SetLives(int32 NewLives);

	/** Get the current wave number */
	UFUNCTION(BlueprintPure, Category = "Game State")
	int32 GetCurrentWave() const { return CurrentWave; }

	/** Set the current wave number */
	UFUNCTION(BlueprintCallable, Category = "Game State")
	void SetCurrentWave(int32 NewWave);

	/** Get the high score for this session */
	UFUNCTION(BlueprintPure, Category = "Game State")
	int32 GetHighScore() const { return HighScore; }

	/** Total enemies defeated across all waves */
	UFUNCTION(BlueprintPure, Category = "Game State")
	int32 GetTotalEnemiesDefeated() const { return TotalEnemiesDefeated; }

	/** Increment enemies defeated count */
	UFUNCTION(BlueprintCallable, Category = "Game State")
	void IncrementEnemiesDefeated();

	/** Check if the game is currently active */
	UFUNCTION(BlueprintPure, Category = "Game State")
	bool IsGameActive() const { return bGameActive; }

	/** Set whether the game is active */
	UFUNCTION(BlueprintCallable, Category = "Game State")
	void SetGameActive(bool bActive);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Game State")
	int32 Score;

	UPROPERTY(BlueprintReadOnly, Category = "Game State")
	int32 Lives;

	UPROPERTY(BlueprintReadOnly, Category = "Game State")
	int32 CurrentWave;

	UPROPERTY(BlueprintReadOnly, Category = "Game State")
	int32 HighScore;

	UPROPERTY(BlueprintReadOnly, Category = "Game State")
	int32 TotalEnemiesDefeated;

	UPROPERTY(BlueprintReadOnly, Category = "Game State")
	bool bGameActive;
};
