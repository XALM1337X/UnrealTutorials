// Fill out your copyright notice in the Description page of Project Settings.
#include "SHealthComp.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SCharacter.h"
#include "Components/PrimitiveComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "SWeapon.h"
#include "SWeaponGrenadeLauncher.h"
#include "BarrelHE.h"
#include "kismet/GameplayStatics.h"
//#include "BarrelExplosionActor.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "GrenadeProjectile.h"
#include "AI/STrackerBot.h"
#include "SGameMode.h"
#include "SPlayerController.h"


// Sets default values for this component's properties
USHealthComp::USHealthComp() {
	//PrimaryComponentTick.bCanEverTick = true;
	this->defaultHealth = 100;
	this->health = 0;
	this->isDead = false;
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void USHealthComp::BeginPlay() {
	Super::BeginPlay();	
	if (GetOwnerRole() == ROLE_Authority) {
		AActor* myOwner = GetOwner();
		myOwner->OnTakePointDamage.AddDynamic(this, &USHealthComp::HandleTakePointDamage);	
		myOwner->OnTakeRadialDamage.AddDynamic(this, &USHealthComp::HandleTakeRadialDamage);
	}
	this->health = this->defaultHealth;	
}

void USHealthComp::HandleTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser ) {
	if (Damage <= 0.0f)	{		
		return;
	}
	//UE_LOG(LogTemp, Log, TEXT("Health Before: %s"), *FString::SanitizeFloat(health));
	this->health = FMath::Clamp(this->health - Damage, 0.0f, this->defaultHealth);
	//UE_LOG(LogTemp, Log, TEXT("Health After: %s"), *FString::SanitizeFloat(health));

	if (this->health <= 0.0f && !this->isDead) {
		ASCharacter* character = Cast<ASCharacter>(DamagedActor);

		if (character) {

			UPawnMovementComponent* MC = character->GetMovementComponent();
			if (MC) {
				MC->StopMovementImmediately();
			} 
			ASPlayerController* cont = Cast<ASPlayerController>(character->GetController());
			if (cont) {
				character->DisableInput(cont);
			}
			ApplyPhysicsPointDamage(character, ShotFromDirection, BoneName);


			ASGameMode* GM = Cast<ASGameMode>(GetWorld()->GetAuthGameMode());                    
			if (GM) {
				if (InstigatedBy) {
					GM->ScoreBroadcast(DamagedActor, DamageCauser, InstigatedBy);
				}
			}

			FTimerHandle Handler;
			GetWorld()->GetTimerManager().SetTimer(Handler, this,&USHealthComp::CleanUp, 3.0f, false);
		}
		this->isDead = true;
	}
	ClientOnHealthChange(this, this->health, Damage, InstigatedBy,  DamageCauser, ShotFromDirection, BoneName);
	//UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(health));

	//Using Broadcast to trigger blueprint
}

void USHealthComp::ClientOnHealthChange_Implementation(USHealthComp* HealthComp, float current_health, float healthDelta, class AController* InstigatedBy, AActor* DamageCauser, FVector ShotFromDirection, FName BoneName) {
	onHealthChanged.Broadcast(this, current_health, healthDelta, InstigatedBy,  DamageCauser, ShotFromDirection, BoneName);
}

//TODO: Fix Damage/Health here.
//As it stands its putting off too much damage at a distance
void USHealthComp::HandleTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser ) {

	if (Damage <= 0.0f)	{		
		return;
	}
	this->health = FMath::Clamp(this->health - Damage, 0.0f, this->defaultHealth);
	
	if (this->health <= 0.0f) {
		ASCharacter* character = Cast<ASCharacter>(DamagedActor);

		if (character) {

			UPawnMovementComponent* MC = character->GetMovementComponent();
			if (MC) {
				MC->StopMovementImmediately();
			} 
			ASPlayerController* cont = Cast<ASPlayerController>(character->GetController());
			if (cont) {
				character->DisableInput(cont);
			}

			if (DamageCauser->IsA(AGrenadeProjectile::StaticClass())) {
				AGrenadeProjectile* gren = Cast<AGrenadeProjectile>(DamageCauser);
				if (gren) {
					ApplyPhysicsRadialDamage(character, Origin, gren->ExplosionSphere->GetScaledSphereRadius(), gren->ExplosionForce);
				}
			} else if (DamageCauser->IsA(ABarrelHE::StaticClass())) {
				ABarrelHE* barrel = Cast<ABarrelHE>(DamageCauser);
				if (barrel) {
					ApplyPhysicsRadialDamage(character, Origin, barrel->ExplosionSphere->GetScaledSphereRadius(), barrel->ExplosionForce);
				}
			} else if (DamageCauser->IsA(ASTrackerBot::StaticClass())) {
				ASTrackerBot* tracker_bot = Cast<ASTrackerBot>(DamageCauser);
				if (tracker_bot) {
					ApplyPhysicsRadialDamage(character, Origin, tracker_bot->ExplosionRadiusSphere->GetScaledSphereRadius(), tracker_bot->ExplosionForce);
				}
			}

			FTimerHandle Handler;
			GetWorld()->GetTimerManager().SetTimer(Handler, this,&USHealthComp::CleanUp, 3.0f, false);
		}
		
	}

	
	FName defaultBone = "defaultBone";
	ClientOnHealthChange(this, this->health, Damage, InstigatedBy,  DamageCauser, Origin, defaultBone);

	
}
//

void USHealthComp::CleanUp() {
	AActor* myOwner = GetOwner();
	ASCharacter* character = Cast<ASCharacter>(myOwner);
	if (character) {
		ASWeapon* wep = character->GetWeapon();
		wep->Destroy();
	}
	if (myOwner) {
		myOwner->Destroy();
	}
}

void USHealthComp::SetHealth(float val) {
	this->health = val;
	ClientOnHealthChange(this, this->health, 0.0f, nullptr,  nullptr, FVector(0.0f, 0.0f, 0.0f), "");
}

float USHealthComp::GetHealth() {
	return this->health;
}


void USHealthComp::ApplyPhysicsPointDamage_Implementation(ASCharacter* character, FVector ShotFromDirection, FName BoneName) {

	USkeletalMeshComponent* weapon_mesh = character->GetWeapon()->GetWeaponMesh();
	if (weapon_mesh) {
		weapon_mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		weapon_mesh->SetSimulatePhysics(true);			
	}
	USkeletalMeshComponent* mesh = character->GetMesh();
	if (mesh) {
		mesh->SetSimulatePhysics(true);
		mesh->AddImpulseAtLocation(ShotFromDirection*15000, ShotFromDirection ,BoneName);   //TODO: Remove magic number later with adjustable UPROPERTY.
	} 
}

void USHealthComp::ApplyPhysicsRadialDamage_Implementation(ASCharacter* character, FVector ExplosionOrigin, float SphereRadius, float ExplosionForce) {

	USkeletalMeshComponent* weapon_mesh = character->GetWeapon()->GetWeaponMesh();
	if (weapon_mesh) {
		weapon_mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		weapon_mesh->SetSimulatePhysics(true);
		weapon_mesh->AddRadialImpulse(ExplosionOrigin, SphereRadius, ExplosionForce/2, ERadialImpulseFalloff::RIF_Constant, true);
	}

	USkeletalMeshComponent* mesh = character->GetMesh();
	if (mesh) {
		mesh->SetSimulatePhysics(true);
		//mesh->AddRadialForce(ExplosionOrigin, SphereRadius, ExplosionForce, ERadialImpulseFalloff::RIF_Constant, true);
		mesh->AddRadialImpulse(ExplosionOrigin, SphereRadius, ExplosionForce, ERadialImpulseFalloff::RIF_Constant, true);	
	}
	
}

void USHealthComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USHealthComp, health);
}
