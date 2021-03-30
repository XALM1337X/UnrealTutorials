// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPickupZone.generated.h"

class USphereComponent;
class UDecalComponent;

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
//Member Variables
protected:

	UPROPERTY(VisibleAnywhere, Category="Components")
	USphereComponent* SphereComp; 
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	UDecalComponent* DecalComp;
	

};
