// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SWeaponGrenadeLauncher.generated.h"

class AGrenadeProjectile;
class USkeletalMeshComponent;

/**
 * 
 */
class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;

UCLASS()
class COOPGAME_API ASWeaponGrenadeLauncher : public ASWeapon
{
	GENERATED_BODY()

public:
	ASWeaponGrenadeLauncher();
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AGrenadeProjectile> ProjectileClass;


protected:
	virtual void Fire_Implementation() override;
	virtual void BeginPlay() override;

private:

};
