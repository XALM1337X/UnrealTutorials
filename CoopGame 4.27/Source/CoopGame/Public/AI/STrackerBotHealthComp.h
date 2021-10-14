// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SHealthComp.h"
#include "STrackerBotHealthComp.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API USTrackerBotHealthComp : public USHealthComp
{
	GENERATED_BODY()

	//Methods
	protected:
		virtual void BeginPlay() override;

		UFUNCTION()
		void HandleTakePointDamageTrackerBot(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser );
		
		UFUNCTION()
		void HandleTakeRadialDamageTrackerBot(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

	//Member vars
	protected:

	public:
		USTrackerBotHealthComp();
	
	public: 

};
