// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExplosionActor.h"
#include "BarrelExplosionActor.generated.h"

/**
 * 
 */

class AController;
UCLASS()
class COOPGAME_API ABarrelExplosionActor : public AExplosionActor
{
	GENERATED_BODY()
	
	//Functions/methods
	protected:
		virtual void BeginPlay() override;
	
	//Members
	protected:
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
		TSubclassOf<UDamageType> damageTypeHEBarrel;

		AController* instigator; 

	//Functions/methods
	public:
		ABarrelExplosionActor();	
		void Init(AController* controller);
		virtual void Tick(float DeltaTime) override;
		virtual void Explode_Implementation() override;
	
	//Members
	public:
	
};
