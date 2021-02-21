// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SHealthComp.h"
#include "SHealthCompMisc.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API USHealthCompMisc : public USHealthComp
{
	GENERATED_BODY()
	
	protected:

	protected:
		virtual void HandleTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser) override;
		virtual void BeginPlay() override;
	public:

	public:
		USHealthCompMisc();
};
