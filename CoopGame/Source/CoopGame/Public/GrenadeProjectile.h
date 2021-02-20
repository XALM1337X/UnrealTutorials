// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrenadeProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystem;
class UStaticMeshComponent;


UCLASS()
class COOPGAME_API AGrenadeProjectile : public AActor
{
	GENERATED_BODY()
	

//Public Functions	
public:	
	// Sets default values for this actor's properties
	AGrenadeProjectile();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
//Protected functions
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
//Public member variables
public:	

	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

//Protected member variables.
protected:

	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ExplosionRadius")
		UStaticMeshComponent* ExplosionMesh;
	
	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
		USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<AActor> ActorToSpawn;

	int TickCount;
};
