// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeaponGrenadeLauncher.h"
#include "GrenadeProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "kismet/GameplayStatics.h"
#include "SCharacter.h"
#include "Net/UnrealNetwork.h"
ASWeaponGrenadeLauncher::ASWeaponGrenadeLauncher() {
	this->meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshCompGren"));
	this->muzzleSocketName = "MuzzleSocket";
	this->currentAmmo = 1;
	this->maxClipSize = 1;
	this->totalAmmo = 3;
	this->needReload = false;
	this->weaponName = "Launcher";
	SetReplicates(true);
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


bool ASWeaponGrenadeLauncher::ServerFire_Validate() { 
	return true;
}


void ASWeaponGrenadeLauncher::ServerFire_Implementation() {
	this->Fire();
}

void ASWeaponGrenadeLauncher::Fire() {
	if (GetLocalRole() != ROLE_Authority) {
		this->ServerFire();
		return;
	}
	if (this->currentAmmo <= 0) {
		this->needReload = true;
		return;
	}
	AActor* myOwner = GetOwner();
	if (myOwner) {
		APawn*  pawn = Cast<APawn>(myOwner);
		if (pawn) {
			if (projectileClass) {	
				myOwner->GetActorEyesViewPoint(eyeLocation, eyeRotation);


				shotDirection = eyeRotation.Vector();

				traceEndPos = eyeLocation + (shotDirection * 10000);

				muzzleLocation = meshComp->GetSocketLocation(muzzleSocketName);
				FRotator muzzleRotator = eyeRotation;


				traceEndPoint = traceEndPos;
				if (GetWorld()->LineTraceSingleByChannel(hit, eyeLocation, traceEndPos, ECC_Visibility)) {
					traceEndPoint = hit.ImpactPoint;
				}


				// spawn the projectile at the muzzle
				FRotator finalRot  =  (traceEndPoint - muzzleLocation).Rotation();
				FActorSpawnParameters actorSpawnParams;
				actorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
				actorSpawnParams.Instigator = pawn;
				//Use replicated muzzleLocation/finalRot
				AGrenadeProjectile* gren = GetWorld()->SpawnActor<AGrenadeProjectile>(projectileClass, muzzleLocation, finalRot, actorSpawnParams);
				gren->Init(pawn->GetController());
				this->needReload = true;
				this->currentAmmo--;
				PlayFireEffects();
			}
		}
	}	
}

void ASWeaponGrenadeLauncher::PlayFireEffects()
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

USkeletalMeshComponent* ASWeaponGrenadeLauncher::GetWeaponMesh() {
	return this->meshComp;
}


void ASWeaponGrenadeLauncher::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME_CONDITION(ASWeapon, TraceStruct, COND_SkipOwner);
	DOREPLIFETIME(ASWeaponGrenadeLauncher, projectileClass);

}
