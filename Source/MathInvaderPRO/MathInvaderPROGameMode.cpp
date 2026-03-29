// Copyright 2024 Peteresi Games. All Rights Reserved.

#include "MathInvaderPROGameMode.h"
#include "MathInvaderPROGameState.h"
#include "MathInvaderPROPlayerController.h"
#include "MathInvaderPROCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AMathInvaderPROGameMode::AMathInvaderPROGameMode()
	: TotalWaves(5)
	, StartingLives(3)
	, BaseScorePerKill(100)
	, ComboMultiplier(1.5f)
	, DifficultyLevel(EDifficultyLevel::Easy)
	, OperationType(EMathOperationType::Addition)
	, WaveDelaySeconds(3.0f)
	, GameOverLevelName(TEXT("Level_GameOver"))
	, VictoryLevelName(TEXT("Level_Victory"))
	, CurrentWave(0)
	, CurrentScore(0)
	, CurrentLives(3)
	, ConsecutiveCorrect(0)
{
	DefaultPawnClass = AMathInvaderPROCharacter::StaticClass();
	PlayerControllerClass = AMathInvaderPROPlayerController::StaticClass();
	GameStateClass = AMathInvaderPROGameState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
}

void AMathInvaderPROGameMode::BeginPlay()
{
	Super::BeginPlay();

	CurrentLives = StartingLives;
	CurrentScore = 0;
	CurrentWave = 0;
	ConsecutiveCorrect = 0;

	// Create and configure math question manager
	MathQuestionManager = NewObject<UMathQuestionManager>(this);
	MathQuestionManager->SetDifficulty(DifficultyLevel);
	MathQuestionManager->SetOperationType(OperationType);

	// Start the first wave
	GetWorldTimerManager().SetTimer(
		WaveStartTimerHandle,
		this,
		&AMathInvaderPROGameMode::StartNextWave,
		WaveDelaySeconds,
		false
	);
}

void AMathInvaderPROGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMathInvaderPROGameMode::StartNextWave()
{
	CurrentWave++;

	// Increase difficulty every two waves
	if (CurrentWave > 2 && DifficultyLevel == EDifficultyLevel::Easy)
	{
		DifficultyLevel = EDifficultyLevel::Medium;
		MathQuestionManager->SetDifficulty(DifficultyLevel);
	}
	else if (CurrentWave > 4 && DifficultyLevel == EDifficultyLevel::Medium)
	{
		DifficultyLevel = EDifficultyLevel::Hard;
		MathQuestionManager->SetDifficulty(DifficultyLevel);
	}

	OnWaveStarted.Broadcast(CurrentWave);
}

void AMathInvaderPROGameMode::OnWaveCleared()
{
	OnWaveCompleted.Broadcast(CurrentWave);

	if (CurrentWave >= TotalWaves)
	{
		TriggerVictory();
	}
	else
	{
		// Start next wave after delay
		GetWorldTimerManager().SetTimer(
			WaveStartTimerHandle,
			this,
			&AMathInvaderPROGameMode::StartNextWave,
			WaveDelaySeconds,
			false
		);
	}
}

void AMathInvaderPROGameMode::AddScore(int32 Points)
{
	CurrentScore += Points;

	// Update game state
	if (AMathInvaderPROGameState* GS = GetGameState<AMathInvaderPROGameState>())
	{
		GS->SetScore(CurrentScore);
	}
}

void AMathInvaderPROGameMode::LoseLife()
{
	CurrentLives--;

	// Update game state
	if (AMathInvaderPROGameState* GS = GetGameState<AMathInvaderPROGameState>())
	{
		GS->SetLives(CurrentLives);
	}

	if (CurrentLives <= 0)
	{
		TriggerGameOver();
	}
}

void AMathInvaderPROGameMode::OnCorrectAnswer(int32 EnemyValue)
{
	ConsecutiveCorrect++;

	// Calculate score with combo multiplier; use Max(1, ...) to guarantee multiplier >= 1.0
	float Multiplier = 1.0f + (FMath::Max(1, ConsecutiveCorrect) - 1) * (ComboMultiplier - 1.0f);
	int32 Points = FMath::RoundToInt(BaseScorePerKill * EnemyValue * Multiplier);
	AddScore(Points);
}

void AMathInvaderPROGameMode::OnWrongAnswer()
{
	ConsecutiveCorrect = 0;
	LoseLife();
}

void AMathInvaderPROGameMode::TriggerGameOver()
{
	OnGameOver.Broadcast();

	// Load game over level after a short delay
	FTimerHandle GameOverTimer;
	GetWorldTimerManager().SetTimer(
		GameOverTimer,
		[this]()
		{
			UGameplayStatics::OpenLevel(this, GameOverLevelName);
		},
		2.0f,
		false
	);
}

void AMathInvaderPROGameMode::TriggerVictory()
{
	OnVictory.Broadcast();

	// Load victory level after a short delay
	FTimerHandle VictoryTimer;
	GetWorldTimerManager().SetTimer(
		VictoryTimer,
		[this]()
		{
			UGameplayStatics::OpenLevel(this, VictoryLevelName);
		},
		2.0f,
		false
	);
}
