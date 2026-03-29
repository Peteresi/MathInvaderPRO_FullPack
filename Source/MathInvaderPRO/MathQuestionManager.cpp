// Copyright 2024 Peteresi Games. All Rights Reserved.

#include "MathQuestionManager.h"
#include "Math/UnrealMathUtility.h"

UMathQuestionManager::UMathQuestionManager()
	: CurrentDifficulty(EDifficultyLevel::Easy)
	, CurrentOperationType(EMathOperationType::Addition)
	, NumberOfChoices(4)
	, CorrectAnswersCount(0)
	, TotalQuestionsCount(0)
{
}

FMathQuestion UMathQuestionManager::GenerateQuestion()
{
	FMathQuestion Question;
	int32 OperandA = 0;
	int32 OperandB = 0;
	GetOperandsForDifficulty(OperandA, OperandB);

	EMathOperationType Operation = GetRandomOperation();

	int32 AnswerRange = 10;

	switch (Operation)
	{
		case EMathOperationType::Addition:
			Question.QuestionText = FString::Printf(TEXT("%d + %d = ?"), OperandA, OperandB);
			Question.CorrectAnswer = OperandA + OperandB;
			AnswerRange = FMath::Max(5, Question.CorrectAnswer / 3);
			break;

		case EMathOperationType::Subtraction:
			// Ensure non-negative result for easier difficulties
			if (OperandA < OperandB && CurrentDifficulty != EDifficultyLevel::Expert)
			{
				Swap(OperandA, OperandB);
			}
			Question.QuestionText = FString::Printf(TEXT("%d - %d = ?"), OperandA, OperandB);
			Question.CorrectAnswer = OperandA - OperandB;
			AnswerRange = FMath::Max(5, FMath::Abs(Question.CorrectAnswer) / 3 + 5);
			break;

		case EMathOperationType::Multiplication:
			Question.QuestionText = FString::Printf(TEXT("%d x %d = ?"), OperandA, OperandB);
			Question.CorrectAnswer = OperandA * OperandB;
			AnswerRange = FMath::Max(10, Question.CorrectAnswer / 4);
			break;

		case EMathOperationType::Division:
		{
			// Ensure clean division
			if (OperandB == 0) OperandB = 1;
			int32 Quotient = OperandA / OperandB;
			OperandA = Quotient * OperandB; // Make divisible
			Question.QuestionText = FString::Printf(TEXT("%d / %d = ?"), OperandA, OperandB);
			Question.CorrectAnswer = Quotient;
			AnswerRange = FMath::Max(5, Quotient / 2 + 3);
			break;
		}

		default:
			Question.QuestionText = FString::Printf(TEXT("%d + %d = ?"), OperandA, OperandB);
			Question.CorrectAnswer = OperandA + OperandB;
			break;
	}

	// Build answer choices array with correct answer and distractors
	TArray<int32> Choices;
	Choices.Add(Question.CorrectAnswer);
	GenerateWrongChoices(Choices, Question.CorrectAnswer, AnswerRange);

	// Shuffle choices
	for (int32 i = Choices.Num() - 1; i > 0; i--)
	{
		int32 j = FMath::RandRange(0, i);
		Choices.Swap(i, j);
	}

	// Trim to desired number of choices
	while (Choices.Num() > NumberOfChoices)
	{
		Choices.RemoveAt(Choices.Num() - 1);
	}

	// Ensure correct answer is still in choices
	if (!Choices.Contains(Question.CorrectAnswer))
	{
		Choices[0] = Question.CorrectAnswer;
	}

	Question.AnswerChoices = Choices;
	Question.CorrectChoiceIndex = Choices.Find(Question.CorrectAnswer);

	TotalQuestionsCount++;
	return Question;
}

bool UMathQuestionManager::CheckAnswer(const FMathQuestion& Question, int32 ChoiceIndex)
{
	bool bCorrect = ChoiceIndex == Question.CorrectChoiceIndex;
	if (bCorrect)
	{
		CorrectAnswersCount++;
	}
	return bCorrect;
}

bool UMathQuestionManager::CheckAnswerValue(const FMathQuestion& Question, int32 AnswerValue)
{
	bool bCorrect = AnswerValue == Question.CorrectAnswer;
	if (bCorrect)
	{
		CorrectAnswersCount++;
	}
	return bCorrect;
}
{
	return AnswerValue == Question.CorrectAnswer;
}

float UMathQuestionManager::GetAccuracyPercentage() const
{
	if (TotalQuestionsCount == 0)
	{
		return 0.0f;
	}
	return (static_cast<float>(CorrectAnswersCount) / static_cast<float>(TotalQuestionsCount)) * 100.0f;
}

void UMathQuestionManager::ResetStats()
{
	CorrectAnswersCount = 0;
	TotalQuestionsCount = 0;
}

void UMathQuestionManager::SetDifficulty(EDifficultyLevel NewDifficulty)
{
	CurrentDifficulty = NewDifficulty;
}

void UMathQuestionManager::SetOperationType(EMathOperationType NewType)
{
	CurrentOperationType = NewType;
}

void UMathQuestionManager::GetOperandsForDifficulty(int32& OutOperandA, int32& OutOperandB) const
{
	switch (CurrentDifficulty)
	{
		case EDifficultyLevel::Easy:
			OutOperandA = FMath::RandRange(1, 10);
			OutOperandB = FMath::RandRange(1, 10);
			break;

		case EDifficultyLevel::Medium:
			OutOperandA = FMath::RandRange(1, 20);
			OutOperandB = FMath::RandRange(1, 20);
			break;

		case EDifficultyLevel::Hard:
			OutOperandA = FMath::RandRange(1, 50);
			OutOperandB = FMath::RandRange(1, 50);
			break;

		case EDifficultyLevel::Expert:
			OutOperandA = FMath::RandRange(1, 100);
			OutOperandB = FMath::RandRange(1, 100);
			break;

		default:
			OutOperandA = FMath::RandRange(1, 10);
			OutOperandB = FMath::RandRange(1, 10);
			break;
	}
}

void UMathQuestionManager::GenerateWrongChoices(TArray<int32>& Choices, int32 CorrectAnswer, int32 Range) const
{
	int32 Attempts = 0;
	const int32 MaxAttempts = 100;

	while (Choices.Num() < NumberOfChoices && Attempts < MaxAttempts)
	{
		Attempts++;
		int32 Offset = FMath::RandRange(-Range, Range);
		if (Offset == 0) Offset = 1;

		int32 WrongAnswer = CorrectAnswer + Offset;

		// Avoid duplicates and avoid negative answers for easy/medium
		if (!Choices.Contains(WrongAnswer))
		{
			if (CurrentDifficulty == EDifficultyLevel::Easy && WrongAnswer < 0)
			{
				continue;
			}
			Choices.Add(WrongAnswer);
		}
	}
}

EMathOperationType UMathQuestionManager::GetRandomOperation() const
{
	if (CurrentOperationType != EMathOperationType::Mixed)
	{
		return CurrentOperationType;
	}

	// For mixed, pick a random operation (excluding Mixed itself)
	int32 RandomOp = FMath::RandRange(0, 3);
	return static_cast<EMathOperationType>(RandomOp);
}
