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
static int32 DebugMode = 0;
FAutoConsoleVariableRef DebugWeapon(TEXT("COOP.DebugWeapons"), DebugMode, TEXT("Draw debugs for weapons"), ECVF_Cheat);
// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("meshComp"));
	RootComponent = meshComp;
	muzzleSocketName = "MuzzleSocket";
	
	needReload = false;
	currentAmmo = 35;
	totalAmmo = 175;
	maxClipSize = 35;
	weaponMod = 13;
	baseDamage = 20;
	weaponName = "rifle";
	clipsLeft = totalAmmo / maxClipSize;
}

void ASWeapon::CallFire()
{

	this->Fire();
	this->currentAmmo--;
	if (this->currentAmmo == 0)
	{
		this->needReload = true;
	}
}

void ASWeapon::ReloadWeapon()
{
	if (this->clipsLeft != 0)
	{
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

int ASWeapon::GetWeaponMod()
{
	return this->weaponMod;
}


int ASWeapon::GetCurrentAmmoCount()
{
	return this->currentAmmo;
}

void ASWeapon::SetCurrentAmmoCount(int count)
{
	this->currentAmmo = count;
}

int ASWeapon::GetMaxClipSize()
{
	return this->maxClipSize;
}

bool ASWeapon::GetReloadState()
{
	return this->needReload;
}

void ASWeapon::SetReloadState(bool reload)
{
	this->needReload = reload;

}

int ASWeapon::GetRemainingClips()
{
	return this->clipsLeft;
}

void ASWeapon::Fire_Implementation()
{

	AActor* myOwner = GetOwner();

	if (myOwner)
	{
		FVector eyeLocation; 
		FRotator eyeRotation;
		
		myOwner->GetActorEyesViewPoint(eyeLocation, eyeRotation);

		FVector shotDirection = eyeRotation.Vector();
		
		FVector traceEndPos = eyeLocation + (shotDirection * 10000);
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(myOwner);
		queryParams.AddIgnoredActor(this);
		queryParams.bTraceComplex = true;
		queryParams.bReturnPhysicalMaterial = true;

		FVector traceEndPoint = traceEndPos;

		FHitResult hit;
		if (GetWorld()->LineTraceSingleByChannel(hit, eyeLocation, traceEndPos, COLLISION_WEAPON, queryParams))
		{
			AActor* hitActor = hit.GetActor();
			// Process damage and such

			EPhysicalSurface surfaceType = UPhysicalMaterial::DetermineSurfaceType(hit.PhysMaterial.Get());
			UParticleSystem* selectedEffect = nullptr;
			FVector scale;
			float actualDamage = baseDamage;
			switch (surfaceType)
			{
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
					actualDamage *= 4;
					break;
				default: 
					selectedEffect = defaultImpactEffect;
					scale.X = 1;
					scale.Y = 1;
					scale.Z = 1;
					break;
			}
			
			if (selectedEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), selectedEffect, hit.ImpactPoint, hit.ImpactNormal.Rotation(), scale);
			}
			traceEndPoint = hit.ImpactPoint;
			UGameplayStatics::ApplyPointDamage(hitActor,actualDamage, shotDirection, hit, myOwner->GetInstigatorController(), this, damageType);
		}
		PlayEffects(eyeLocation, traceEndPos, traceEndPoint);
	}
}

void ASWeapon::PlayEffects(FVector eyeLocation, FVector traceEndPos, FVector traceEndPoint)
{
		if (muzzleEffect) 
		{
			UGameplayStatics::SpawnEmitterAttached(muzzleEffect, meshComp, muzzleSocketName);
		}		

		if (tracerEffect) 
		{
			FVector muzzleLocation = meshComp->GetSocketLocation(muzzleSocketName);
			UParticleSystemComponent* tracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), tracerEffect, muzzleLocation);
			if (tracerComp)
			{
				tracerComp->SetVectorParameter("BeamEnd", traceEndPoint);
			}
			if (DebugMode > 0) {
				DrawDebugLine(GetWorld(), eyeLocation, traceEndPos, FColor::White, false, 1.0f, 0, 1.0f);
			}
		}
		APawn* owner = Cast<APawn>(GetOwner());
		if (owner)
		{
			APlayerController* PC = Cast<APlayerController>(owner->GetController());
			if (PC)
			{
				if (fireCamShake)
				{
					PC->ClientPlayCameraShake(fireCamShake);
				}
			}
		}
}