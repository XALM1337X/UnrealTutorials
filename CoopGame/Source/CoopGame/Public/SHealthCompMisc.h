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

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealthComponents")
		float healthBarrelHE;
		
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealthComponents")
		float defaultHealthBarrelHE;
	protected:
		UFUNCTION()
		void HandleTakePointDamageBarrelHE(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);
		UFUNCTION()
		void HandleTakeRadialDamageBarrelHE(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser ); 
		virtual void BeginPlay() override;
	public:

	public:
		USHealthCompMisc();
		float GetHealth();
};
