// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SWeaponGrenadeLauncher.generated.h"

class AGrenadeProjectile;
class USkeletalMeshComponent;
class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class ASCharacter;

UCLASS()
class COOPGAME_API ASWeaponGrenadeLauncher : public ASWeapon
{
	GENERATED_BODY()

public:

	ASWeaponGrenadeLauncher();
	
	virtual void SetCurrentAmmoCount(int) override;
	virtual bool GetReloadState() override;
	virtual void SetReloadState(bool) override;

	virtual void ServerReloadWeapon_Implementation() override;
	virtual void ReloadWeapon() override;

	UFUNCTION(NetMulticast, reliable)
	virtual void PlayFireEffectsGren();

	
	virtual int GetMaxClipSize() override;
	virtual int GetCurrentAmmoCount() override;
	virtual USkeletalMeshComponent* GetWeaponMesh() override;
	virtual void ServerFire_Implementation() override;
	virtual bool ServerFire_Validate() override;
	
	virtual void ClientOnAmmoChanged_Implementation(ASCharacter* my_char, int ammoCount, int clipCount, int clipSize, const FString& weapon_name) override;
	virtual void Fire();

public:

	UPROPERTY(Replicated, EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AGrenadeProjectile> projectileClass;


protected:
	//virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
protected:

};
