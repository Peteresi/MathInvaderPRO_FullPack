// Copyright 2024 Peteresi Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MathQuestionManager.generated.h"

UENUM(BlueprintType)
enum class EMathOperationType : uint8
{
	Addition		UMETA(DisplayName = "Addition (+)"),
	Subtraction		UMETA(DisplayName = "Subtraction (-)"),
	Multiplication	UMETA(DisplayName = "Multiplication (x)"),
	Division		UMETA(DisplayName = "Division (/)"),
	Mixed			UMETA(DisplayName = "Mixed")
};

UENUM(BlueprintType)
enum class EDifficultyLevel : uint8
{
	Easy	UMETA(DisplayName = "Easy"),
	Medium	UMETA(DisplayName = "Medium"),
	Hard	UMETA(DisplayName = "Hard"),
	Expert	UMETA(DisplayName = "Expert")
};

USTRUCT(BlueprintType)
struct FMathQuestion
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Math Question")
	FString QuestionText;

	UPROPERTY(BlueprintReadOnly, Category = "Math Question")
	int32 CorrectAnswer;

	UPROPERTY(BlueprintReadOnly, Category = "Math Question")
	TArray<int32> AnswerChoices;

	UPROPERTY(BlueprintReadOnly, Category = "Math Question")
	int32 CorrectChoiceIndex;

	FMathQuestion()
		: QuestionText(TEXT(""))
		, CorrectAnswer(0)
		, CorrectChoiceIndex(0)
	{}
};

/**
 * Manages generation and validation of math questions for MathInvaderPRO.
 * Generates questions based on difficulty level and math operation type.
 */
UCLASS(BlueprintType, Blueprintable)
class MATHINVADERPRO_API UMathQuestionManager : public UObject
{
	GENERATED_BODY()

public:
	UMathQuestionManager();

	/** Generate a new math question based on current difficulty and operation settings */
	UFUNCTION(BlueprintCallable, Category = "Math Question")
	FMathQuestion GenerateQuestion();

	/** Check if the provided answer index is correct */
	UFUNCTION(BlueprintCallable, Category = "Math Question")
	bool CheckAnswer(const FMathQuestion& Question, int32 ChoiceIndex);

	/** Check if the provided answer value is correct */
	UFUNCTION(BlueprintCallable, Category = "Math Question")
	bool CheckAnswerValue(const FMathQuestion& Question, int32 AnswerValue);

	/** Get the number of questions answered correctly this session */
	UFUNCTION(BlueprintPure, Category = "Math Question")
	int32 GetCorrectAnswersCount() const { return CorrectAnswersCount; }

	/** Get the total number of questions asked this session */
	UFUNCTION(BlueprintPure, Category = "Math Question")
	int32 GetTotalQuestionsCount() const { return TotalQuestionsCount; }

	/** Get accuracy percentage (0-100) */
	UFUNCTION(BlueprintPure, Category = "Math Question")
	float GetAccuracyPercentage() const;

	/** Reset session statistics */
	UFUNCTION(BlueprintCallable, Category = "Math Question")
	void ResetStats();

	/** Set difficulty level */
	UFUNCTION(BlueprintCallable, Category = "Math Question")
	void SetDifficulty(EDifficultyLevel NewDifficulty);

	/** Set math operation type */
	UFUNCTION(BlueprintCallable, Category = "Math Question")
	void SetOperationType(EMathOperationType NewType);

	/** Current difficulty level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Math Question|Settings")
	EDifficultyLevel CurrentDifficulty;

	/** Current operation type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Math Question|Settings")
	EMathOperationType CurrentOperationType;

	/** Number of answer choices to generate */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Math Question|Settings", meta = (ClampMin = "2", ClampMax = "6"))
	int32 NumberOfChoices;

protected:
	/** Generate operands based on difficulty */
	void GetOperandsForDifficulty(int32& OutOperandA, int32& OutOperandB) const;

	/** Generate wrong answer choices that are plausible but incorrect */
	void GenerateWrongChoices(TArray<int32>& Choices, int32 CorrectAnswer, int32 Range) const;

	/** Get a random operation based on current settings */
	EMathOperationType GetRandomOperation() const;

private:
	int32 CorrectAnswersCount;
	int32 TotalQuestionsCount;
};
