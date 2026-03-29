// Copyright 2024 Peteresi Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MathQuestionManager.h"
#include "MathInvaderPROPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnswerSubmitted, bool, bCorrect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyTargeted, class AMathInvaderPROEnemy*, TargetEnemy);

/**
 * Player Controller for MathInvaderPRO.
 * Handles input, targeting enemies, and submitting math answers.
 */
UCLASS()
class MATHINVADERPRO_API AMathInvaderPROPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMathInvaderPROPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// ---- Targeting ----

	/** Set the currently targeted enemy */
	UFUNCTION(BlueprintCallable, Category = "Player|Targeting")
	void SetTargetEnemy(class AMathInvaderPROEnemy* NewTarget);

	/** Get the currently targeted enemy */
	UFUNCTION(BlueprintPure, Category = "Player|Targeting")
	AMathInvaderPROEnemy* GetTargetEnemy() const { return TargetEnemy; }

	/** Automatically target the nearest or most dangerous enemy */
	UFUNCTION(BlueprintCallable, Category = "Player|Targeting")
	void AutoTargetEnemy();

	// ---- Answer Submission ----

	/** Submit an answer by choice index (0-based) */
	UFUNCTION(BlueprintCallable, Category = "Player|Math")
	void SubmitAnswerByChoice(int32 ChoiceIndex);

	/** Submit an answer by direct value */
	UFUNCTION(BlueprintCallable, Category = "Player|Math")
	void SubmitAnswerByValue(int32 AnswerValue);

	/** Get the current active math question (for the targeted enemy) */
	UFUNCTION(BlueprintPure, Category = "Player|Math")
	FMathQuestion GetCurrentQuestion() const { return CurrentQuestion; }

	/** Request a new question for the targeted enemy */
	UFUNCTION(BlueprintCallable, Category = "Player|Math")
	void RequestNewQuestion();

	// ---- UI / HUD ----

	/** Show the main game HUD */
	UFUNCTION(BlueprintCallable, Category = "Player|HUD")
	void ShowGameHUD();

	/** Show the pause menu */
	UFUNCTION(BlueprintCallable, Category = "Player|HUD")
	void ShowPauseMenu();

	/** Hide all menus and restore game HUD */
	UFUNCTION(BlueprintCallable, Category = "Player|HUD")
	void HidePauseMenu();

	// ---- Events ----

	UPROPERTY(BlueprintAssignable, Category = "Player|Events")
	FOnAnswerSubmitted OnAnswerSubmitted;

	UPROPERTY(BlueprintAssignable, Category = "Player|Events")
	FOnEnemyTargeted OnEnemyTargeted;

	/** HUD widget class to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|HUD")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	/** Pause menu widget class to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|HUD")
	TSubclassOf<class UUserWidget> PauseMenuWidgetClass;

protected:
	void HandleFireInput();
	void HandlePauseInput();
	void HandleMoveRight(float AxisValue);
	void HandleSelectAnswer1();
	void HandleSelectAnswer2();
	void HandleSelectAnswer3();
	void HandleSelectAnswer4();

	UPROPERTY()
	AMathInvaderPROEnemy* TargetEnemy;

	UPROPERTY()
	class UUserWidget* HUDWidget;

	UPROPERTY()
	class UUserWidget* PauseMenuWidget;

	FMathQuestion CurrentQuestion;

	bool bIsPaused;
};
