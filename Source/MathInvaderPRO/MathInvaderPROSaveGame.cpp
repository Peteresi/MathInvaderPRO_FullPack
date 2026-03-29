// Copyright 2024 Peteresi Games. All Rights Reserved.

#include "MathInvaderPROSaveGame.h"
#include "Kismet/GameplayStatics.h"

const FString UMathInvaderPROSaveGame::SaveSlotName = TEXT("MathInvaderPROSave");
const int32 UMathInvaderPROSaveGame::SaveUserIndex = 0;

UMathInvaderPROSaveGame::UMathInvaderPROSaveGame()
	: PreferredDifficulty(EDifficultyLevel::Easy)
	, PreferredOperationType(EMathOperationType::Addition)
	, MasterVolume(1.0f)
	, MusicVolume(0.8f)
	, SFXVolume(1.0f)
	, bTutorialCompleted(false)
	, TotalGamesPlayed(0)
	, TotalCorrectAnswers(0)
	, TotalQuestionsAnswered(0)
{
}

bool UMathInvaderPROSaveGame::SaveGame(UMathInvaderPROSaveGame* SaveGameData)
{
	if (!SaveGameData) return false;
	return UGameplayStatics::SaveGameToSlot(SaveGameData, SaveSlotName, SaveUserIndex);
}

UMathInvaderPROSaveGame* UMathInvaderPROSaveGame::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex))
	{
		return Cast<UMathInvaderPROSaveGame>(
			UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex));
	}

	// Return a fresh save game if none exists
	return Cast<UMathInvaderPROSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UMathInvaderPROSaveGame::StaticClass()));
}

void UMathInvaderPROSaveGame::AddHighScore(const FPlayerHighScore& NewScore)
{
	HighScores.Add(NewScore);

	// Sort by score descending
	HighScores.Sort([](const FPlayerHighScore& A, const FPlayerHighScore& B)
	{
		return A.Score > B.Score;
	});

	// Keep only top 10
	if (HighScores.Num() > 10)
	{
		HighScores.SetNum(10);
	}
}

int32 UMathInvaderPROSaveGame::GetTopScore() const
{
	if (HighScores.Num() == 0) return 0;
	return HighScores[0].Score;
}
