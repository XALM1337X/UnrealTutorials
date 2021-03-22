// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"


class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class UCameraShake;

USTRUCT()
struct FHitScanTrace {
	GENERATED_BODY()
	public: 

		UPROPERTY() 
		TEnumAsByte<EPhysicalSurface> SurfaceTypeBytes;

		UPROPERTY() 
		FVector_NetQuantize TraceTo;

		UPROPERTY() 
		int Ticker;
};

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
//Public memeber variables
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName muzzleSocketName;

	UPROPERTY(BlueprintReadOnly)
	FString weaponName;

//Public member functions
public:	
	// Sets default values for this actor's properties
	ASWeapon();
	virtual void BeginPlay() override;

	virtual void SetCurrentAmmoCount(int);
	virtual bool GetReloadState();
	virtual float GetTimeBetweenShots();
	virtual void SetLastFireTime(float);
	virtual float GetLastFireTime();
	virtual void SetReloadState(bool);
	virtual void SetFireRate(float);
	virtual void ReloadWeapon();

	UFUNCTION(Server, Reliable) //TODO: Add validation later.
	virtual void ServerReloadWeapon();

	virtual void PlayFireEffects();
	virtual void PlayImpactEffects();

	virtual USkeletalMeshComponent* GetWeaponMesh();

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerFire();
	
	virtual void Fire();

	UFUNCTION(BlueprintCallable)
	virtual int GetMaxClipSize();

	UFUNCTION(BlueprintCallable)
	virtual int GetCurrentAmmoCount();

	UFUNCTION(BlueprintCallable)
	virtual int GetRemainingClips();

	UFUNCTION(Client, Reliable)
	virtual void ClientOnAmmoChanged(ASCharacter* my_char, int ammoCount, int clipCount, int clipSize, const FString& weapon_name);

	
//Protected member variables
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USkeletalMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<UDamageType> damageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* muzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* defaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* fleshImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* tracerEffect;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<UCameraShake> fireCamShake;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	int baseDamage;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float rateOfFire;
	
	//These three are testing remove if things fuck up.
	UPROPERTY(EditDefaultsOnly)
	FVector eyeLocation;
	
	UPROPERTY(EditDefaultsOnly)
	FVector traceEndPos;
	
	UPROPERTY(EditDefaultsOnly)
	FVector traceEndPoint;
	//////////////////////////////////////////////////

	UPROPERTY(EditDefaultsOnly)
	FVector muzzleLocation;

	UPROPERTY(EditDefaultsOnly)
	FRotator eyeRotation;
	
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EPhysicalSurface> surfaceType;
	
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* selectedEffect;

	UPROPERTY(EditDefaultsOnly)
	FVector scale;

	UPROPERTY(EditDefaultsOnly)
	FVector shotDirection; 

	UPROPERTY(EditDefaultsOnly)
	FHitResult hit;

	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace TraceStruct;

	FCollisionQueryParams queryParams;
	
	int currentAmmo;	
	
	int totalAmmo;
	
	int maxClipSize;
	
	int clipsLeft;
	
	bool needReload;
	
	int weaponMod;
	float timeBetweenShots;
	float lastFireTime;

//Protected member functions
protected:

	UFUNCTION()
	void OnRep_HitScanTrace();

	//void Tick(float DeltaTime);

};
