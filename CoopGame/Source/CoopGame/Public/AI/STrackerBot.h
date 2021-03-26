// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"


class UStaticMeshComponent;
class USTrackerBotHealthComp;
class USphereComponent;


UCLASS()
class COOPGAME_API ASTrackerBot : public APawn {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTrackerBot();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(Server, Reliable)
	void ServerExplode();

	void Explode();

//Member variables
public:	
	UPROPERTY(EditAnywhere, Category = "Effects")
	UStaticMeshComponent* mesh;
	
	UMaterialInstanceDynamic* MatInst;

	UPROPERTY(EditAnywhere)
	USphereComponent* ExplosionRadiusSphere;

	UPROPERTY(EditAnywhere)
	USphereComponent* ExplosionDetinationRangeSphere;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionForce; 


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector GetNextPathPoint();

	UFUNCTION(NetMulticast, Reliable)
	void PlayExplosionEffect();
	
	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditDefaultsOnly)
	USTrackerBotHealthComp* TrackerBotHealthComp;
	
	FVector NextPathPoint;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	bool bUseVelocityChange;
	
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float MovementForce;
	
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float RequiredDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	int ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	FVector ExplosionAnimationScale;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	UParticleSystem*  ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<UDamageType> damageType;

};
