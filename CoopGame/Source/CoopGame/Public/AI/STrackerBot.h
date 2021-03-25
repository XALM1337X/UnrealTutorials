// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"


class UStaticMeshComponent;
class USTrackerBotHealthComp;


UCLASS()
class COOPGAME_API ASTrackerBot : public APawn {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTrackerBot();
	virtual void Tick(float DeltaTime) override;

//Member variables
public:	
	UPROPERTY(EditAnywhere, Category = "Effects")
	UStaticMeshComponent* mesh;
	
	UMaterialInstanceDynamic* MatInst;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector GetNextPathPoint();

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

};
