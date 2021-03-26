// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BarrelHE.generated.h"

class USkeletalMeshComponent;
class USHealthCompMisc;
class AController;
class UMaterialInterface;
class UStaticMeshComponent;
class USphereComponent;
class UDamageType;
class USoundCue;

UCLASS()
class COOPGAME_API ABarrelHE : public AActor {
	GENERATED_BODY()


//Methods/Functions
protected:
	virtual void BeginPlay() override;

//Members
protected:

	USHealthCompMisc* HealthComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
	TSubclassOf<UDamageType> damageType;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ExplosionEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	FVector ExplosionAnimationScale;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundCue* ExplosionSound;

//Methods/Functions
public:	
	ABarrelHE();
	
	UFUNCTION(Server, Reliable)
	void ServerExplode();

	UFUNCTION(NetMultiCast,Reliable)
	void CleanUp();
	
	UFUNCTION(NetMultiCast, Reliable)
	void PlayExplosionEffect();

//Members
public:	
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* OffMaterial;
	
	bool isDead;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	float ExplosionDamage;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* ExplosionSphere;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float ExplosionForce;

	FTimerHandle Handler;
};
