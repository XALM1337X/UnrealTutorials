// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosionActor.generated.h"

class USphereComponent;
class UParticleSystemComponent;

UCLASS()
class COOPGAME_API AExplosionActor : public AActor
{
	GENERATED_BODY()
	

protected: 
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USphereComponent* SphereComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* Explosion;

	FVector ExplosionAnimationScale;
	int ExplosionForce;

public:	
	// Sets default values for this actor's properties
	AExplosionActor();
	int Ticks;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(Server,Reliable,WithValidation)
	virtual void Explode();

};
