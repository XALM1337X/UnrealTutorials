// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BarrelHE.generated.h"

class USkeletalMeshComponent;
class USHealthCompMisc;

UCLASS()
class COOPGAME_API ABarrelHE : public AActor
{
	GENERATED_BODY()


//Methods/Functions
protected:
	virtual void BeginPlay() override;

//Members
protected:
/*
UPROPERTY(VisibleDefaultsOnly, Category= "Components")
USkeletalMeshComponent* meshComp;
*/

USHealthCompMisc* Health;

UPROPERTY(EditAnywhere, Category = "Spawning")
TSubclassOf<AActor> ActorToSpawn;

//Methods/Functions
public:	
	ABarrelHE();

//Members
public:	
	virtual void Tick(float DeltaTime) override;

};
