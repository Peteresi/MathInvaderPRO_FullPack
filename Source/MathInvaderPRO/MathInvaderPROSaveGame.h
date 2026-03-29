// Copyright 2024 Peteresi Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MathQuestionManager.h"
#include "MathInvaderPROSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FPlayerHighScore
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Save Game")
	int32 Score;

	UPROPERTY(BlueprintReadWrite, Category = "Save Game")
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite, Category = "Save Game")
	FDateTime AchievedDate;

	UPROPERTY(BlueprintReadWrite, Category = "Save Game")
	EDifficultyLevel Difficulty;

	FPlayerHighScore()
		: Score(0)
		, PlayerName(TEXT("Player"))
		, Difficulty(EDifficultyLevel::Easy)
	{}
};

/**
 * Save game data for MathInvaderPRO.
 * Stores high scores, player preferences, and progress.
 */
UCLASS()
class MATHINVADERPRO_API UMathInvaderPROSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UMathInvaderPROSaveGame();

	/** Slot name used for saving */
	static const FString SaveSlotName;

	/** User index for this save */
	static const int32 SaveUserIndex;

	/** Save the current state to disk */
	static bool SaveGame(UMathInvaderPROSaveGame* SaveGameData);

	/** Load saved state from disk */
	static UMathInvaderPROSaveGame* LoadGame();

	/** High scores list (top 10) */
	UPROPERTY(BlueprintReadWrite, Category = "Save Game|High Scores")
	TArray<FPlayerHighScore> HighScores;

	/** Player's preferred difficulty */
	UPROPERTY(BlueprintReadWrite, Category = "Save Game|Settings")
	EDifficultyLevel PreferredDifficulty;

	/** Player's preferred operation type */
	UPROPERTY(BlueprintReadWrite, Category = "Save Game|Settings")
	EMathOperationType PreferredOperationType;

	/** Master volume (0.0 - 1.0) */
	UPROPERTY(BlueprintReadWrite, Category = "Save Game|Audio")
	float MasterVolume;

	/** Music volume (0.0 - 1.0) */
	UPROPERTY(BlueprintReadWrite, Category = "Save Game|Audio")
	float MusicVolume;

	/** SFX volume (0.0 - 1.0) */
	UPROPERTY(BlueprintReadWrite, Category = "Save Game|Audio")
	float SFXVolume;

	/** Whether the player has completed the tutorial */
	UPROPERTY(BlueprintReadWrite, Category = "Save Game|Progress")
	bool bTutorialCompleted;

	/** Total games played */
	UPROPERTY(BlueprintReadWrite, Category = "Save Game|Stats")
	int32 TotalGamesPlayed;

	/** Total correct answers across all sessions */
	UPROPERTY(BlueprintReadWrite, Category = "Save Game|Stats")
	int32 TotalCorrectAnswers;

	/** Total questions answered across all sessions */
	UPROPERTY(BlueprintReadWrite, Category = "Save Game|Stats")
	int32 TotalQuestionsAnswered;

	/** Add a new high score, keeping only the top 10 */
	UFUNCTION(BlueprintCallable, Category = "Save Game|High Scores")
	void AddHighScore(const FPlayerHighScore& NewScore);

	/** Get the top score */
	UFUNCTION(BlueprintPure, Category = "Save Game|High Scores")
	int32 GetTopScore() const;
};
