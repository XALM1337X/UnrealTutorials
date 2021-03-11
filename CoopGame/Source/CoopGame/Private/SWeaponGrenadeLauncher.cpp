// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeaponGrenadeLauncher.h"
#include "GrenadeProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "kismet/GameplayStatics.h"
#include "SCharacter.h"
#include "Net/UnrealNetwork.h"
ASWeaponGrenadeLauncher::ASWeaponGrenadeLauncher() {
	//PrimaryActorTick.bCanEverTick = true;
	this->meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshCompGren"));
	//NOTE: HOLY FUCK DONT FORGET TO ENABLE THIS OR ELSE CLIENTS GETSOCKETLOCATION WILL BE FUCKED UP, ALSO HAD TO ENABLE THIS ON CHARACTER BLUEPRINT.
	this->meshComp->VisibilityBasedAnimTickOption  = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	this->muzzleSocketName = "MuzzleSocketGren";
	this->currentAmmo = 1;
	this->maxClipSize = 1;
	this->totalAmmo = 3;
	this->needReload = false;
	this->weaponName = "Launcher";
	SetReplicates(true);
}


void ASWeaponGrenadeLauncher::BeginPlay() {
	Super::BeginPlay();
}
/*void ASWeaponGrenadeLauncher::Tick(float DeltaTime) {

} */

void ASWeaponGrenadeLauncher::ServerReloadWeapon_Implementation() {
	this->ReloadWeapon();
}

void ASWeaponGrenadeLauncher::ReloadWeapon() {
	
	if (GetLocalRole() != ROLE_Authority) {
		this->ServerReloadWeapon();
		return;
	}
	if (this->clipsLeft != 0)
	{
		this->currentAmmo = this->maxClipSize;
		this->needReload = false;
		this->clipsLeft--;
	}	
	AActor* myOwner = GetOwner();
	if (myOwner) {
		ASCharacter* my_char = Cast<ASCharacter>(myOwner);
		ClientOnAmmoChanged(my_char, this->currentAmmo, this->clipsLeft, this->maxClipSize, this->weaponName);
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
				this->needReload = true;
				this->currentAmmo--;	
				myOwner->GetActorEyesViewPoint(eyeLocation, eyeRotation);


				shotDirection = eyeRotation.Vector();

				traceEndPos = eyeLocation + (shotDirection * 10000);
				muzzleLocation = this->meshComp->GetSocketLocation(muzzleSocketName);
				FRotator muzzleRotator = eyeRotation;


				traceEndPoint = traceEndPos;
				if (GetWorld()->LineTraceSingleByChannel(hit, eyeLocation, traceEndPoint, ECC_Visibility)) {
					traceEndPoint = hit.ImpactPoint;
				}


				// spawn the projectile at the muzzle
				FRotator finalRot  =  (traceEndPoint - muzzleLocation).Rotation();
				FActorSpawnParameters actorSpawnParams;
				actorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				actorSpawnParams.Instigator = pawn;
				//Use replicated muzzleLocation/finalRot
				AGrenadeProjectile* gren = GetWorld()->SpawnActor<AGrenadeProjectile>(projectileClass, this->muzzleLocation+5, finalRot, actorSpawnParams);
			
				if (GetLocalRole() == ROLE_Authority) {
					ASCharacter* my_char = Cast<ASCharacter>(pawn);
					if (my_char) {
						this->ClientOnAmmoChanged(my_char, this->currentAmmo, this->clipsLeft, this->maxClipSize, this->weaponName);
					}
				}			
				PlayFireEffectsGren();
			}
		}
	}	
}

void ASWeaponGrenadeLauncher::ClientOnAmmoChanged_Implementation(ASCharacter* my_char, int ammoCount, int clipCount, int clipSize, const FString& weapon_name) {
	my_char->onAmmoChanged.Broadcast(ammoCount, clipCount, clipSize, weapon_name);
}

void ASWeaponGrenadeLauncher::PlayFireEffectsGren_Implementation() {
	if (muzzleEffect) 
	{
		UGameplayStatics::SpawnEmitterAttached(muzzleEffect, meshComp, muzzleSocketName);
	}	
	
	APawn* owner = Cast<APawn>(GetOwner());
	if (owner){
		APlayerController* PC = Cast<APlayerController>(owner->GetController());
		if (PC)	{
			if (fireCamShake) {
				PC->ClientPlayCameraShake(fireCamShake);
			} else {
				UE_LOG(LogTemp, Warning, TEXT("FIRECAM_SHAKE_ERR"));
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
