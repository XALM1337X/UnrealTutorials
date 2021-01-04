// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "DrawDebugHelpers.h"
#include "kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"

static int32 DebugMode = 0;
FAutoConsoleVariableRef DebugWeapon(TEXT("COOP.DebugWeapons"), DebugMode, TEXT("Draw debugs for weapons"), ECVF_Cheat);
// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MuzzleSocketName = "MuzzleSocket";
	
	needReload = false;
	currentAmmo = 45;
	maxAmmo = 45;
	clipsLeft = 5;
	weaponMod = 15;
	weaponName = "rifle";
}

void ASWeapon::CallFire()
{

	this->Fire();
	this->currentAmmo--;
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

bool ASWeapon::GetReloadState()
{
	return this->needReload;
}

void ASWeapon::SetReloadState(bool reload)
{
	this->needReload = reload;

}

void ASWeapon::Fire_Implementation()
{

	AActor* MyOwner = GetOwner();

	if (MyOwner)
	{
		FVector EyeLocation; 
		FRotator EyeRotation;
		
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();

		FVector TraceEndPos = EyeLocation + (ShotDirection * 10000);

		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(MyOwner);
		queryParams.AddIgnoredActor(this);
		queryParams.bTraceComplex = true;

		FVector TraceEndPoint = TraceEndPos;

		FHitResult hit;
		if (GetWorld()->LineTraceSingleByChannel(hit, EyeLocation, TraceEndPos, ECC_Visibility))
		{
			AActor* HitActor = hit.GetActor();
			// Process damage and such

			UGameplayStatics::ApplyPointDamage(HitActor,20.0f, ShotDirection, hit, MyOwner->GetInstigatorController(), this, DamageType);
			if (ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, hit.ImpactPoint, hit.ImpactNormal.Rotation());
			}
			TraceEndPoint = hit.ImpactPoint;
		}

		if (MuzzleEffect) 
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
		}		


		if (TracerEffect) 
		{
			FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
			UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
			if (TracerComp)
			{
				TracerComp->SetVectorParameter("BeamEnd", TraceEndPoint);
			}
			if (DebugMode > 0) {
				DrawDebugLine(GetWorld(), EyeLocation, TraceEndPos, FColor::White, false, 1.0f, 0, 1.0f);
			}
		}

	}
}

