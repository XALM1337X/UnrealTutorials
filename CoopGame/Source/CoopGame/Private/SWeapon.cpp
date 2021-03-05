// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "DrawDebugHelpers.h"
#include "kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "SCharacter.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "CoopGame/CoopGame.h"
#include "Net/UnrealNetwork.h"

static int32 DebugMode = 0;
FAutoConsoleVariableRef DebugWeapon(TEXT("COOP.DebugWeapons"), DebugMode, TEXT("Draw debugs for weapons"), ECVF_Cheat);
// Sets default values
ASWeapon::ASWeapon() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("meshComp"));
	RootComponent = meshComp;
	muzzleSocketName = "MuzzleSocket";
	
	needReload = false;
	currentAmmo = 35;
	totalAmmo = 175;
	maxClipSize = 35;
	weaponMod = 13;
	baseDamage = 8;
	weaponName = "rifle";
	clipsLeft = totalAmmo / maxClipSize;
	rateOfFire = 600;
	SetReplicates(true);

	//Built in actor tick network frequency.
	//Note: This is ranged from 2.0f to 100.0f by default.
	NetUpdateFrequency = 66.0f;		//
	MinNetUpdateFrequency = 33.0f;	//
}

void ASWeapon::BeginPlay() {
	Super::BeginPlay();
	this->timeBetweenShots = 60/rateOfFire;
}

void ASWeapon::ReloadWeapon() {
	if (this->clipsLeft != 0) {
		if (this->currentAmmo == 0 && this->totalAmmo >= this->maxClipSize) {
			this->currentAmmo = this->maxClipSize;
			this->totalAmmo -= this->maxClipSize;
		} else if (this->currentAmmo != 0 && this->totalAmmo >= this->maxClipSize){
			this->totalAmmo -= this->maxClipSize;
			this->totalAmmo += this->currentAmmo;
			this->currentAmmo = this->maxClipSize;
		}

		this->needReload = false;
		this->clipsLeft = this->totalAmmo/this->maxClipSize;
	}
}

int ASWeapon::GetCurrentAmmoCount() {
	return this->currentAmmo;
}

void ASWeapon::SetCurrentAmmoCount(int count) {
	this->currentAmmo = count;
}

int ASWeapon::GetMaxClipSize() {
	return this->maxClipSize;
}

bool ASWeapon::GetReloadState() {
	return this->needReload;
}

void ASWeapon::SetReloadState(bool reload) {
	this->needReload = reload;

}

int ASWeapon::GetRemainingClips() {
	return this->clipsLeft;
}

float ASWeapon::GetTimeBetweenShots() {
	return this->timeBetweenShots;
}

void ASWeapon::SetLastFireTime(float value) {
	this->lastFireTime = value;
}

float ASWeapon::GetLastFireTime() {
	return this->lastFireTime;
}

void ASWeapon::SetFireRate(float value) {
	this->rateOfFire = value;
}

void ASWeapon::ServerFire_Implementation() { 
	this->Fire();
}

bool ASWeapon::ServerFire_Validate() { 
	return true; 
}

void ASWeapon::Fire() {
	
	if (GetLocalRole() < ROLE_Authority) {	
		this->ServerFire();
	} 

	if (this->currentAmmo <= 0 ) {
		this->needReload = true;
		return;
	}
	
	if (GetCurrentAmmoCount() > 0 && !GetReloadState()) {
		this->currentAmmo--;
		AActor* myOwner = GetOwner();
		if (myOwner) {		

			myOwner->GetActorEyesViewPoint(eyeLocation, eyeRotation);
			shotDirection = eyeRotation.Vector();
			
			traceEndPos = eyeLocation + (shotDirection * 10000);
			queryParams.AddIgnoredActor(myOwner);
			queryParams.AddIgnoredActor(this);
			queryParams.bTraceComplex = true;
			queryParams.bReturnPhysicalMaterial = true;

			traceEndPoint = traceEndPos;

			if (GetWorld()->LineTraceSingleByChannel(hit, eyeLocation, traceEndPos, COLLISION_WEAPON, queryParams)) {
				AActor* hitActor = hit.GetActor();
				traceEndPoint = hit.ImpactPoint;
				// Process damage and such

				surfaceType = UPhysicalMaterial::DetermineSurfaceType(hit.PhysMaterial.Get());
				selectedEffect = nullptr;
				float actualDamage = baseDamage;
				switch (surfaceType) {
					case SURFACE_FLESHDEFAULT:
						break;
					case SURFACE_FLESHVULNERABLE:
						actualDamage *= 4;
						break;
					default: 
						break;
				}
				UGameplayStatics::ApplyPointDamage(hitActor, actualDamage, shotDirection, hit, myOwner->GetInstigatorController(), this, damageType);
			}
			this->SetLastFireTime(GetWorld()->TimeSeconds);
		}
		if (GetLocalRole() == ROLE_Authority) {
			TraceStruct.TraceTo = traceEndPoint;
			TraceStruct.SurfaceTypeBytes = surfaceType;
		}
	} else {
		this->needReload = true;
	}
}

void ASWeapon::OnRep_HitScanTrace() {
	PlayFireEffects();
	PlayImpactEffects();
}


void ASWeapon::PlayImpactEffects() {
	AActor* myOwner = GetOwner();
	if (myOwner) {		
		muzzleLocation = meshComp->GetSocketLocation(muzzleSocketName);
		FVector ShotDirection = TraceStruct.TraceTo - muzzleLocation;
		ShotDirection.Normalize();

		switch (TraceStruct.SurfaceTypeBytes) {
			case SURFACE_FLESHDEFAULT:
				selectedEffect = fleshImpactEffect;
				scale.X = .3;
				scale.Y = .3;
				scale.Z = .3;
				break;
			case SURFACE_FLESHVULNERABLE:
				selectedEffect = fleshImpactEffect;
				scale.X = .7;
				scale.Y = .7;
				scale.Z = .7;
				break;
			default: 
				selectedEffect = defaultImpactEffect;
				scale.X = 1;
				scale.Y = 1;
				scale.Z = 1;
				break;
		}

		if (selectedEffect)	{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), selectedEffect, TraceStruct.TraceTo, ShotDirection.Rotation(), scale);
		}

	}
}

void ASWeapon::PlayFireEffects() {
	AActor* myOwner = GetOwner();
	if (myOwner) {		
		muzzleLocation = meshComp->GetSocketLocation(muzzleSocketName);
	}
	if (muzzleEffect) {
		UGameplayStatics::SpawnEmitterAttached(muzzleEffect, meshComp, muzzleSocketName);
	}		
	if (tracerEffect) {
		UParticleSystemComponent* tracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), tracerEffect, muzzleLocation);
		if (tracerComp)	{
			tracerComp->SetVectorParameter("BeamEnd", TraceStruct.TraceTo);
		}
		if (DebugMode > 0) {
			DrawDebugLine(GetWorld(), eyeLocation, traceEndPos, FColor::White, false, 1.0f, 0, 1.0f);
		}
	}
	APawn* owner = Cast<APawn>(GetOwner());
	if (owner) {
		APlayerController* PC = Cast<APlayerController>(owner->GetController());
		if (PC) {
			if (fireCamShake) {
				PC->ClientPlayCameraShake(fireCamShake);
			}
		}
	}
}

USkeletalMeshComponent* ASWeapon::GetWeaponMesh() {
	return this->meshComp;
}

void ASWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME_CONDITION(ASWeapon, TraceStruct, COND_SkipOwner);
	DOREPLIFETIME(ASWeapon, TraceStruct);
}