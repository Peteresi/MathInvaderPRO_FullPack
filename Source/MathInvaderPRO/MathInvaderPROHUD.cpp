// Copyright 2024 Peteresi Games. All Rights Reserved.

#include "MathInvaderPROHUD.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "TimerManager.h"

AMathInvaderPROHUD::AMathInvaderPROHUD()
	: GameOverlayWidget(nullptr)
	, FeedbackWidget(nullptr)
	, bHUDVisible(true)
{
}

void AMathInvaderPROHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GameOverlayWidgetClass)
	{
		GameOverlayWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), GameOverlayWidgetClass);
		if (GameOverlayWidget)
		{
			GameOverlayWidget->AddToViewport();
		}
	}
}

void AMathInvaderPROHUD::DrawHUD()
{
	Super::DrawHUD();

	// Debug draw fallback text when no widgets are present
#if WITH_EDITOR
	if (!GameOverlayWidget && Canvas)
	{
		FString DebugText = TEXT("MathInvaderPRO - No HUD Widget assigned");
		DrawText(DebugText, FLinearColor::Yellow, Canvas->SizeX / 2.0f - 200.0f, 20.0f);
	}
#endif
}

void AMathInvaderPROHUD::SetHUDVisible(bool bVisible)
{
	bHUDVisible = bVisible;
	if (GameOverlayWidget)
	{
		bVisible ? GameOverlayWidget->SetVisibility(ESlateVisibility::Visible)
				 : GameOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMathInvaderPROHUD::UpdateQuestionDisplay(const FString& QuestionText, const TArray<int32>& Choices)
{
	// This is primarily driven by UMG Blueprints that bind to game state data
	// The C++ HUD triggers the update; Blueprints respond to the event
}

void AMathInvaderPROHUD::ShowAnswerFeedback(bool bCorrect)
{
	if (FeedbackWidgetClass && !FeedbackWidget)
	{
		FeedbackWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), FeedbackWidgetClass);
	}
	if (FeedbackWidget)
	{
		FeedbackWidget->AddToViewport();

		// Auto-hide feedback after 1 second
		GetWorldTimerManager().SetTimer(
			FeedbackTimerHandle,
			this,
			&AMathInvaderPROHUD::HideFeedback,
			1.0f,
			false
		);
	}
}

void AMathInvaderPROHUD::ShowWaveBanner(int32 WaveNumber)
{
	// Wave banner is shown via Blueprint/UMG widget events
	// This C++ function serves as the trigger point
}

void AMathInvaderPROHUD::HideFeedback()
{
	if (FeedbackWidget)
	{
		FeedbackWidget->RemoveFromParent();
	}
}
