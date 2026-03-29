// Copyright 2024 Peteresi Games. All Rights Reserved.

#include "MathInvaderPROGameState.h"

AMathInvaderPROGameState::AMathInvaderPROGameState()
	: Score(0)
	, Lives(3)
	, CurrentWave(0)
	, HighScore(0)
	, TotalEnemiesDefeated(0)
	, bGameActive(false)
{
}

void AMathInvaderPROGameState::SetScore(int32 NewScore)
{
	Score = NewScore;
	if (Score > HighScore)
	{
		HighScore = Score;
	}
}

void AMathInvaderPROGameState::SetLives(int32 NewLives)
{
	Lives = FMath::Max(0, NewLives);
}

void AMathInvaderPROGameState::SetCurrentWave(int32 NewWave)
{
	CurrentWave = NewWave;
}

void AMathInvaderPROGameState::IncrementEnemiesDefeated()
{
	TotalEnemiesDefeated++;
}

void AMathInvaderPROGameState::SetGameActive(bool bActive)
{
	bGameActive = bActive;
}
