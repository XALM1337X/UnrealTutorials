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
UCLASS()
class COOPGAME_API ABarrelHE : public AActor {
	GENERATED_BODY()


//Methods/Functions
protected:
	virtual void BeginPlay() override;

//Members
protected:

USHealthCompMisc* HealthComp;

UPROPERTY(EditAnywhere, Category = "FX")
UStaticMeshComponent* mesh;

//Methods/Functions
public:	
	ABarrelHE();
	void TriggerExplosion(AController* controller);
	void CleanUp();

//Members
public:	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* OffMaterial;
	bool isDead;
};
