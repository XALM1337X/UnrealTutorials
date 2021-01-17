// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeaponGrenadeLauncher.h"
#include "GrenadeProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "kismet/GameplayStatics.h"

ASWeaponGrenadeLauncher::ASWeaponGrenadeLauncher()
{
	meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshCompGren"));
	muzzleSocketName = "MuzzleSocket";
	currentAmmo = 1;
	maxClipSize = 1;
	totalAmmo = 3;
	needReload = false;
	weaponMod = 1;
	weaponName = "Launcher";
}


int ASWeaponGrenadeLauncher::GetWeaponMod() 
{
	return this->weaponMod;
}

void ASWeaponGrenadeLauncher::CallFire() 
{
	this->Fire();
	this->currentAmmo--;
	this->needReload = true;
}

void ASWeaponGrenadeLauncher::ReloadWeapon()
{
	if (this->clipsLeft != 0)
	{
		this->currentAmmo = this->maxClipSize;
		this->needReload = false;
		this->clipsLeft--;
	}
}
int ASWeaponGrenadeLauncher::GetCurrentAmmoCount() 
{
	return this->currentAmmo;
}
int ASWeaponGrenadeLauncher::GetMaxClipSize()
{
	return this->maxClipSize;
}
void ASWeaponGrenadeLauncher::SetCurrentAmmoCount(int ammo) 
{
	this->currentAmmo = ammo;
}

bool ASWeaponGrenadeLauncher::GetReloadState()
{
	return this->needReload;
}

void ASWeaponGrenadeLauncher::SetReloadState(bool reload) {
	this->needReload = reload;
}





void ASWeaponGrenadeLauncher::Fire_Implementation() 
{
	AActor* myOwner = GetOwner();
	APawn*  pawn = Cast<APawn>(myOwner);
	if (myOwner)
	{
		if (projectileClass)
		{
			FVector eyeLocation; 
			FRotator eyeRotation;		
			myOwner->GetActorEyesViewPoint(eyeLocation, eyeRotation);


			FVector shotDirection = eyeRotation.Vector();

			FVector traceEndPos = eyeLocation + (shotDirection * 10000);

			FVector muzzleLocation = meshComp->GetSocketLocation(muzzleSocketName);
			FRotator muzzleRotator = eyeRotation;
			//Set Spawn Collision Handling Override
			FActorSpawnParameters actorSpawnParams;
			actorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;


			FHitResult hit;
			FVector traceEndPoint = traceEndPos;
			if (GetWorld()->LineTraceSingleByChannel(hit, eyeLocation, traceEndPos, ECC_Visibility))
			{
				traceEndPoint = hit.ImpactPoint;
			}

			if (pawn)
			{
				actorSpawnParams.Instigator = pawn;
				// spawn the projectile at the muzzle
				FRotator finalRot  =  (traceEndPoint - muzzleLocation).Rotation();
				GetWorld()->SpawnActor<AGrenadeProjectile>(projectileClass, muzzleLocation, finalRot, actorSpawnParams);
			}
			PlayEffects(eyeLocation, traceEndPos, traceEndPoint);
		}
	}
}

void ASWeaponGrenadeLauncher::PlayEffects(FVector eyeLocation, FVector traceEndPos, FVector traceEndPoint)
{
	if (muzzleEffect) 
	{
		UGameplayStatics::SpawnEmitterAttached(muzzleEffect, meshComp, muzzleSocketName);
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

