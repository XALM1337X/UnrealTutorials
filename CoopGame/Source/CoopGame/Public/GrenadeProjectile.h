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
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UFUNCTION(NetMulticast, Reliable)
	void PlayExplosionEffect();

	UFUNCTION(Server, Reliable)
	void ServerExplode();

	UFUNCTION()
	void Explode();
	
//Public member variables
public:	

	UPROPERTY(Replicated, VisibleDefaultsOnly)
	FTimerHandle ExplodeTimer;


//Protected functions
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//Protected member variables.
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ExplosionRadius")
	UStaticMeshComponent* GrenMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<UDamageType> damageType;
	
	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* CollisionComp;
	

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;


		
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Effects")
	UParticleSystem* Explosion;
	
	/* Collision Sphere for Explosion*/
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Components")
	USphereComponent* ExplosionSphere;

	//TODO: This needs to be in replicated struct FVector_NetQuantize
	FVector ExplosionAnimationScale;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Effects")
	float ExplosionForce;

};
