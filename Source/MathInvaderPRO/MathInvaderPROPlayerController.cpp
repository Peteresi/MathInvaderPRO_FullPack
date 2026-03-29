// Copyright 2024 Peteresi Games. All Rights Reserved.

#include "MathInvaderPROPlayerController.h"
#include "MathInvaderPROEnemy.h"
#include "MathInvaderPROGameMode.h"
#include "MathInvaderPROCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

AMathInvaderPROPlayerController::AMathInvaderPROPlayerController()
	: TargetEnemy(nullptr)
	, HUDWidget(nullptr)
	, PauseMenuWidget(nullptr)
	, bIsPaused(false)
{
}

void AMathInvaderPROPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ShowGameHUD();
}

void AMathInvaderPROPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAction("Fire", IE_Pressed, this, &AMathInvaderPROPlayerController::HandleFireInput);
		InputComponent->BindAction("Pause", IE_Pressed, this, &AMathInvaderPROPlayerController::HandlePauseInput);
		InputComponent->BindAction("SelectAnswer_1", IE_Pressed, this, &AMathInvaderPROPlayerController::HandleSelectAnswer1);
		InputComponent->BindAction("SelectAnswer_2", IE_Pressed, this, &AMathInvaderPROPlayerController::HandleSelectAnswer2);
		InputComponent->BindAction("SelectAnswer_3", IE_Pressed, this, &AMathInvaderPROPlayerController::HandleSelectAnswer3);
		InputComponent->BindAction("SelectAnswer_4", IE_Pressed, this, &AMathInvaderPROPlayerController::HandleSelectAnswer4);
		InputComponent->BindAxis("MoveRight", this, &AMathInvaderPROPlayerController::HandleMoveRight);
	}
}

void AMathInvaderPROPlayerController::SetTargetEnemy(AMathInvaderPROEnemy* NewTarget)
{
	TargetEnemy = NewTarget;
	OnEnemyTargeted.Broadcast(NewTarget);
	RequestNewQuestion();
}

void AMathInvaderPROPlayerController::AutoTargetEnemy()
{
	// Find the enemy that has advanced the furthest (lowest Y position in top-down 2D game)
	AMathInvaderPROEnemy* BestTarget = nullptr;
	float LowestY = FLT_MAX;

	for (TActorIterator<AMathInvaderPROEnemy> It(GetWorld()); It; ++It)
	{
		AMathInvaderPROEnemy* Enemy = *It;
		if (IsValid(Enemy))
		{
			float EnemyY = Enemy->GetActorLocation().Y;
			if (EnemyY < LowestY)
			{
				LowestY = EnemyY;
				BestTarget = Enemy;
			}
		}
	}

	if (BestTarget)
	{
		SetTargetEnemy(BestTarget);
	}
}

void AMathInvaderPROPlayerController::SubmitAnswerByChoice(int32 ChoiceIndex)
{
	if (!TargetEnemy) return;

	AMathInvaderPROGameMode* GM = Cast<AMathInvaderPROGameMode>(UGameplayStatics::GetGameMode(this));
	if (!GM) return;

	UMathQuestionManager* QM = GM->GetMathQuestionManager();
	if (!QM) return;

	bool bCorrect = QM->CheckAnswer(CurrentQuestion, ChoiceIndex);
	OnAnswerSubmitted.Broadcast(bCorrect);

	if (bCorrect)
	{
		int32 EnemyValue = TargetEnemy->GetPointValue();
		TargetEnemy->OnCorrectAnswerReceived();
		GM->OnCorrectAnswer(EnemyValue);
		AutoTargetEnemy();
	}
	else
	{
		GM->OnWrongAnswer();
	}

	// Generate next question
	RequestNewQuestion();
}

void AMathInvaderPROPlayerController::SubmitAnswerByValue(int32 AnswerValue)
{
	if (!TargetEnemy) return;

	AMathInvaderPROGameMode* GM = Cast<AMathInvaderPROGameMode>(UGameplayStatics::GetGameMode(this));
	if (!GM) return;

	UMathQuestionManager* QM = GM->GetMathQuestionManager();
	if (!QM) return;

	bool bCorrect = QM->CheckAnswerValue(CurrentQuestion, AnswerValue);
	OnAnswerSubmitted.Broadcast(bCorrect);

	if (bCorrect)
	{
		int32 EnemyValue = TargetEnemy->GetPointValue();
		TargetEnemy->OnCorrectAnswerReceived();
		GM->OnCorrectAnswer(EnemyValue);
		AutoTargetEnemy();
	}
	else
	{
		GM->OnWrongAnswer();
	}

	RequestNewQuestion();
}

void AMathInvaderPROPlayerController::RequestNewQuestion()
{
	AMathInvaderPROGameMode* GM = Cast<AMathInvaderPROGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM && GM->GetMathQuestionManager())
	{
		CurrentQuestion = GM->GetMathQuestionManager()->GenerateQuestion();
	}
}

void AMathInvaderPROPlayerController::ShowGameHUD()
{
	if (HUDWidgetClass && !HUDWidget)
	{
		HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
}

void AMathInvaderPROPlayerController::ShowPauseMenu()
{
	if (bIsPaused) return;

	bIsPaused = true;
	UGameplayStatics::SetGamePaused(this, true);

	if (PauseMenuWidgetClass && !PauseMenuWidget)
	{
		PauseMenuWidget = CreateWidget<UUserWidget>(this, PauseMenuWidgetClass);
	}
	if (PauseMenuWidget)
	{
		PauseMenuWidget->AddToViewport();
	}

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}

void AMathInvaderPROPlayerController::HidePauseMenu()
{
	if (!bIsPaused) return;

	bIsPaused = false;
	UGameplayStatics::SetGamePaused(this, false);

	if (PauseMenuWidget)
	{
		PauseMenuWidget->RemoveFromParent();
	}

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

void AMathInvaderPROPlayerController::HandleFireInput()
{
	// Firing is handled via answer submission in this math game
	if (TargetEnemy)
	{
		// Fire a projectile toward the targeted enemy
		if (APawn* ControlledPawn = GetPawn())
		{
			// Notify the character to fire
			if (AMathInvaderPROCharacter* Character = Cast<AMathInvaderPROCharacter>(ControlledPawn))
			{
				Character->FireProjectile();
			}
		}
	}
}

void AMathInvaderPROPlayerController::HandlePauseInput()
{
	if (bIsPaused)
	{
		HidePauseMenu();
	}
	else
	{
		ShowPauseMenu();
	}
}

void AMathInvaderPROPlayerController::HandleMoveRight(float AxisValue)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisValue);
	}
}

void AMathInvaderPROPlayerController::HandleSelectAnswer1() { SubmitAnswerByChoice(0); }
void AMathInvaderPROPlayerController::HandleSelectAnswer2() { SubmitAnswerByChoice(1); }
void AMathInvaderPROPlayerController::HandleSelectAnswer3() { SubmitAnswerByChoice(2); }
void AMathInvaderPROPlayerController::HandleSelectAnswer4() { SubmitAnswerByChoice(3); }
