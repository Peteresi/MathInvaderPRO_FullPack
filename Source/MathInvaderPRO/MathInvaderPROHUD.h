// Copyright 2024 Peteresi Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MathInvaderPROHUD.generated.h"

/**
 * HUD for MathInvaderPRO.
 * Provides on-screen display for score, lives, wave info and math questions.
 */
UCLASS()
class MATHINVADERPRO_API AMathInvaderPROHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMathInvaderPROHUD();

	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

	/** Show or hide the main gameplay HUD overlay */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetHUDVisible(bool bVisible);

	/** Update the question display */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateQuestionDisplay(const FString& QuestionText, const TArray<int32>& Choices);

	/** Show a feedback message (correct/wrong) */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowAnswerFeedback(bool bCorrect);

	/** Show the wave announcement banner */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowWaveBanner(int32 WaveNumber);

	/** Widget class for the main game overlay UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Widgets")
	TSubclassOf<class UUserWidget> GameOverlayWidgetClass;

	/** Widget class for the answer feedback display */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Widgets")
	TSubclassOf<class UUserWidget> FeedbackWidgetClass;

protected:
	UPROPERTY()
	class UUserWidget* GameOverlayWidget;

	UPROPERTY()
	class UUserWidget* FeedbackWidget;

	bool bHUDVisible;

	FTimerHandle FeedbackTimerHandle;
	void HideFeedback();
};
