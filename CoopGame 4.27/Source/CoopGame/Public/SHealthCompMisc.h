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
		void HandleTakeAnyDamageBarrelHE(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
		
		
		virtual void BeginPlay() override;
	public:

	public:
		USHealthCompMisc();
		float GetHealth();
};
