// Copyright 2024 Peteresi Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MathInvaderPROCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDestroyed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileFired, class AMathInvaderPROProjectile*, Projectile);

/**
 * Player character (spaceship) for MathInvaderPRO.
 * Moves horizontally along the bottom of the screen and fires projectiles.
 */
UCLASS()
class MATHINVADERPRO_API AMathInvaderPROCharacter : public APawn
{
	GENERATED_BODY()

public:
	AMathInvaderPROCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Fire a projectile upward toward enemies */
	UFUNCTION(BlueprintCallable, Category = "Character|Combat")
	void FireProjectile();

	/** Get the character's current health */
	UFUNCTION(BlueprintPure, Category = "Character|Health")
	float GetHealth() const { return Health; }

	/** Apply damage to the character */
	UFUNCTION(BlueprintCallable, Category = "Character|Health")
	void TakeDamageAmount(float DamageAmount);

	/** Whether the character is currently alive */
	UFUNCTION(BlueprintPure, Category = "Character|Health")
	bool IsAlive() const { return Health > 0.0f; }

	/** Enable or disable the shield */
	UFUNCTION(BlueprintCallable, Category = "Character|Shield")
	void SetShieldActive(bool bActive);

	/** Whether the shield is currently active */
	UFUNCTION(BlueprintPure, Category = "Character|Shield")
	bool IsShieldActive() const { return bShieldActive; }

	// ---- Events ----

	UPROPERTY(BlueprintAssignable, Category = "Character|Events")
	FOnPlayerDestroyed OnPlayerDestroyed;

	UPROPERTY(BlueprintAssignable, Category = "Character|Events")
	FOnProjectileFired OnProjectileFired;

	// ---- Components ----

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	class UStaticMeshComponent* ShipMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	class UStaticMeshComponent* ShieldMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	class UArrowComponent* ProjectileSpawnPoint;

	// ---- Settings ----

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Settings")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Settings")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Settings")
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Settings")
	TSubclassOf<class AMathInvaderPROProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Settings")
	float MovementBoundsX;

protected:
	float Health;
	bool bShieldActive;
	bool bCanFire;

	FTimerHandle FireCooldownTimer;

	void OnFireCooldownComplete();
	void MoveRight(float AxisValue);
	void ClampPositionToBounds();
};
