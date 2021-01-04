// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"


class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
//Public memeber variables
public:

//Public member functions
public:	
	// Sets default values for this actor's properties
	ASWeapon();
	virtual void CallFire();
	virtual int GetWeaponMod();
	virtual int GetCurrentAmmoCount();
	virtual void SetCurrentAmmoCount(int);
	virtual bool GetReloadState();
	virtual void SetReloadState(bool);

//Protected member variables
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* ImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* TracerEffect;

	UPROPERTY(BlueprintReadOnly)
	FString weaponName;

	int currentAmmo;
	int maxAmmo;
	int clipsLeft;
	bool needReload;
	int weaponMod;

//Protected member functions
protected:
	UFUNCTION(Server, BlueprintCallable, Reliable)
	virtual void Fire();

};
