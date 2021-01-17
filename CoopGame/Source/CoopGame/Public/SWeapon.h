// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"


class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class UCameraShake;
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
	virtual void SetCurrentAmmoCount(int);
	virtual bool GetReloadState();
	virtual void SetReloadState(bool);
	virtual void ReloadWeapon();
	virtual void PlayEffects(FVector,FVector,FVector);
	
	UFUNCTION(BlueprintCallable)
	virtual int GetMaxClipSize();

	UFUNCTION(BlueprintCallable)
	virtual int GetCurrentAmmoCount();

	UFUNCTION(BlueprintCallable)
	virtual int GetRemainingClips();
	
//Protected member variables
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USkeletalMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<UDamageType> damageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName muzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* muzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* defaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* fleshImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* tracerEffect;

	UPROPERTY(BlueprintReadOnly)
	FString weaponName;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<UCameraShake> fireCamShake;

	int currentAmmo;
	int totalAmmo;
	int maxClipSize;
	int clipsLeft;
	bool needReload;
	int weaponMod;

//Protected member functions
protected:
	UFUNCTION(Server, BlueprintCallable, Reliable)
	virtual void Fire();

};
