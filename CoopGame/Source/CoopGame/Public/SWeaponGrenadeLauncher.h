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
	
	
	virtual void CallFire() override;
	virtual void SetCurrentAmmoCount(int) override;
	virtual bool GetReloadState() override;
	virtual void SetReloadState(bool) override;
	virtual void ReloadWeapon() override;
	virtual void PlayEffects(FVector,FVector,FVector) override;

	
	virtual int GetMaxClipSize() override;
	virtual int GetCurrentAmmoCount() override;
	virtual USkeletalMeshComponent* GetWeaponMesh() override;

public:

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AGrenadeProjectile> projectileClass;


protected:
	virtual void Fire_Implementation() override;
	
protected:

private:

};
