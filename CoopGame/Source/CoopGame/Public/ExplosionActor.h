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
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* Explosion;

	FVector ExplosionAnimationScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<UDamageType> damageType;

	AController* instigator;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Sets default values for this actor's properties
	int Ticks;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USphereComponent* SphereComp;

	float ExplosionForce;

public:	
	// Called every frame
	AExplosionActor();

	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(Server,Reliable,WithValidation)
	virtual void Explode();

	void Init(AController* controller);

};
