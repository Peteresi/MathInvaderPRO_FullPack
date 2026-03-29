// Copyright 2024 Peteresi Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MathInvaderPROEnemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Basic		UMETA(DisplayName = "Basic Invader"),
	Fast		UMETA(DisplayName = "Fast Invader"),
	Tank		UMETA(DisplayName = "Tank Invader"),
	Boss		UMETA(DisplayName = "Boss Invader")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDestroyed, class AMathInvaderPROEnemy*, Enemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyReachedBottom);

/**
 * Enemy invader actor for MathInvaderPRO.
 * Moves downward and side-to-side in formation. 
 * Defeated when the player answers the associated math question correctly.
 */
UCLASS()
class MATHINVADERPRO_API AMathInvaderPROEnemy : public AActor
{
	GENERATED_BODY()

public:
	AMathInvaderPROEnemy();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Called when the player correctly answers the question tied to this enemy */
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void OnCorrectAnswerReceived();

	/** Called when this enemy fires at the player */
	UFUNCTION(BlueprintCallable, Category = "Enemy|Combat")
	void FireAtPlayer();

	/** Get the score/point value of this enemy */
	UFUNCTION(BlueprintPure, Category = "Enemy")
	int32 GetPointValue() const { return PointValue; }

	/** Set the formation position index */
	UFUNCTION(BlueprintCallable, Category = "Enemy|Formation")
	void SetFormationIndex(int32 Row, int32 Column);

	/** Set the movement direction (used by the enemy manager/wave controller) */
	UFUNCTION(BlueprintCallable, Category = "Enemy|Movement")
	void SetMovementDirection(float Direction) { MoveDirection = Direction; }

	/** Step down one row and reverse horizontal direction */
	UFUNCTION(BlueprintCallable, Category = "Enemy|Movement")
	void StepDown();

	/** Get the display text shown on this invader (the math question operand) */
	UFUNCTION(BlueprintPure, Category = "Enemy|Display")
	FString GetDisplayText() const { return DisplayText; }

	/** Set the display text for this enemy */
	UFUNCTION(BlueprintCallable, Category = "Enemy|Display")
	void SetDisplayText(const FString& Text) { DisplayText = Text; }

	// ---- Events ----

	UPROPERTY(BlueprintAssignable, Category = "Enemy|Events")
	FOnEnemyDestroyed OnEnemyDestroyed;

	UPROPERTY(BlueprintAssignable, Category = "Enemy|Events")
	FOnEnemyReachedBottom OnEnemyReachedBottom;

	// ---- Components ----

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Components")
	class UStaticMeshComponent* EnemyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Components")
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Components")
	class UTextRenderComponent* AnswerText;

	// ---- Settings ----

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Settings")
	EEnemyType EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Settings")
	int32 PointValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Settings")
	float HorizontalSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Settings")
	float VerticalStepSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Settings")
	float FireInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Settings")
	float BoundaryX;

	/** Y position at which this enemy has "invaded" - triggers game over if crossed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Settings")
	float BottomBoundaryY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Settings")
	TSubclassOf<class AMathInvaderPROProjectile> ProjectileClass;

protected:
	float MoveDirection;
	FString DisplayText;

	int32 FormationRow;
	int32 FormationColumn;

	FTimerHandle FireTimerHandle;

	void OnFireTimerExpired();
};
