// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrenadeProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystem;
class UStaticMeshComponent;
USTRUCT()
struct FGrenEffectsReplicator {
	GENERATED_BODY()

	public:
		UPROPERTY()
		FVector_NetQuantize net_explosion_scale;

		UPROPERTY()
		FVector_NetQuantize net_actor_loc;
		
		UPROPERTY()
		FRotator net_actor_rot;

};

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

	UFUNCTION(NetMulticast, Reliable)
	void TriggerTimer();

	UFUNCTION()
	void OnRep_FGrenEffectsReplicate();
	
//Public member variables
public:	

	UPROPERTY(VisibleDefaultsOnly)
	FTimerHandle ExplodeTimer;

	/* Collision Sphere for Explosion*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	USphereComponent* ExplosionSphere;

	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float ExplosionForce;


//Protected functions
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

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


		
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* Explosion;
	

	//TODO: This needs to be in replicated struct FVector_NetQuantize
	FVector ExplosionAnimationScale;

	UPROPERTY(ReplicatedUsing=OnRep_FGrenEffectsReplicate)
	FGrenEffectsReplicator grenEffectsRep;

};
