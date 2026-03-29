// Copyright 2024 Peteresi Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MathInvaderPROProjectile.generated.h"

UENUM(BlueprintType)
enum class EProjectileOwner : uint8
{
	Player	UMETA(DisplayName = "Player Projectile"),
	Enemy	UMETA(DisplayName = "Enemy Projectile")
};

/**
 * Projectile actor for MathInvaderPRO.
 * Used by both the player ship and enemy invaders.
 */
UCLASS()
class MATHINVADERPRO_API AMathInvaderPROProjectile : public AActor
{
	GENERATED_BODY()

public:
	AMathInvaderPROProjectile();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Initialize projectile direction and speed */
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void InitProjectile(FVector Direction, float Speed);

	/** Who fired this projectile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	EProjectileOwner ProjectileOwner;

	/** Damage dealt on hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Damage;

	/** Speed of the projectile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileSpeed;

	/** Maximum lifetime before auto-destroy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float MaxLifetime;

	// ---- Components ----

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Components")
	class UCapsuleComponent* CollisionCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Components")
	class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile|Components")
	class UParticleSystemComponent* TrailEffect;

protected:
	FVector MoveDirection;
	float AliveTime;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
