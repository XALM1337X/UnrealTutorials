// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPickupZone.generated.h"

class USphereComponent;
class UDecalComponent;
class APickupPowerBase;

UCLASS()
class COOPGAME_API ASPickupZone : public AActor {
	GENERATED_BODY()

//Methods/Functions
public:	
	// Sets default values for this actor's properties
	ASPickupZone();
	
	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

//Member Variables
public:	


//Methods/Functions
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Respawn();
//Member Variables
protected:

	UPROPERTY(VisibleAnywhere, Category="Components")
	USphereComponent* SphereComp; 
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	UDecalComponent* DecalComp;

	UPROPERTY(EditDefaultsOnly , Category="PickupActor")
	TSubclassOf<APickupPowerBase> PowerUpBase;

	UPROPERTY(EditDefaultsOnly , Category="PickupActor")
	float CoolDownDuration;

	APickupPowerBase* PowerUpInstance;

	FTimerHandle TimerHandlerRespawn;




	

};
