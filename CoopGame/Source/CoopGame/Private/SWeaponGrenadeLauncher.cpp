// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeaponGrenadeLauncher.h"
#include "GrenadeProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "kismet/GameplayStatics.h"

ASWeaponGrenadeLauncher::ASWeaponGrenadeLauncher()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshCompGren"));
	MuzzleSocketName = "MuzzleSocket";
	currentAmmo = 1;
	clipsLeft = 2;
	maxAmmo = 1;
	needReload = false;
	weaponMod = 1;
	weaponName = "grenadeLauncher";
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
		this->currentAmmo = this->maxAmmo;
		this->needReload = false;
		this->clipsLeft--;
	}
}
int ASWeaponGrenadeLauncher::GetCurrentAmmoCount() 
{
	return this->currentAmmo;
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
	AActor* MyOwner = GetOwner();
	APawn*  pawn = Cast<APawn>(MyOwner);
	if (MyOwner)
	{
		if (ProjectileClass)
		{
			FVector EyeLocation; 
			FRotator EyeRotation;		
			MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);


			FVector ShotDirection = EyeRotation.Vector();

			FVector TraceEndPos = EyeLocation + (ShotDirection * 10000);

			FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
			FRotator MuzzleRotator = EyeRotation;
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;


			FHitResult hit;
			FVector TraceEndPoint = TraceEndPos;
			if (GetWorld()->LineTraceSingleByChannel(hit, EyeLocation, TraceEndPos, ECC_Visibility))
			{
				//AActor* HitActor = hit.GetActor();
				TraceEndPoint = hit.ImpactPoint;

			}

			if (pawn)
			{
				ActorSpawnParams.Instigator = pawn;
				// spawn the projectile at the muzzle
				FRotator FinalRot  =  (TraceEndPoint - MuzzleLocation).Rotation();
				GetWorld()->SpawnActor<AGrenadeProjectile>(ProjectileClass, MuzzleLocation, FinalRot, ActorSpawnParams);
			}
			PlayEffects(EyeLocation, TraceEndPos, TraceEndPoint);
		}

	}
}

void ASWeaponGrenadeLauncher::PlayEffects(FVector EyeLocation, FVector TraceEndPos, FVector TraceEndPoint)
{
		if (MuzzleEffect) 
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
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

