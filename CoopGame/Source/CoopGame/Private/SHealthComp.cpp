// Fill out your copyright notice in the Description page of Project Settings.
#include "SHealthComp.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SCharacter.h"
#include "Components/PrimitiveComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "SWeapon.h"
#include "kismet/GameplayStatics.h"
#include "BarrelExplosionActor.h"
#include "Components/SphereComponent.h"


// Sets default values for this component's properties
USHealthComp::USHealthComp()
{
	//PrimaryComponentTick.bCanEverTick = true;
	this->defaultHealth = 100;
	this->health = 0;
}


// Called when the game starts
void USHealthComp::BeginPlay()
{
	Super::BeginPlay();
	AActor* myOwner = GetOwner();
	this->health = this->defaultHealth;
	Role = myOwner->GetLocalRole();
	myOwner->OnTakePointDamage.AddDynamic(this, &USHealthComp::HandleTakePointDamage);	
	myOwner->OnTakeRadialDamage.AddDynamic(this, &USHealthComp::HandleTakeRadialDamage);

}

void USHealthComp::HandleTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser ) 
{
	if (Damage <= 0.0f)
	{		
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
			APlayerController* cont = Cast<APlayerController>(character->GetController());
			if (cont) {
				character->DisableInput(cont);
			}
			USkeletalMeshComponent* mesh = character->GetMesh();
			if (mesh) {
				mesh->SetSimulatePhysics(true);
				mesh->AddImpulseAtLocation(ShotFromDirection*8000, ShotFromDirection ,BoneName);   //TODO: Remove magic number later with adjustable UPROPERTY.
			}
			USkeletalMeshComponent* weapon_mesh = character->GetWeapon()->GetWeaponMesh();
			if (weapon_mesh) {
				weapon_mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
				weapon_mesh->SetSimulatePhysics(true);
				//weapon_mesh->WakeRigidBody();
				UE_LOG(LogTemp, Log, TEXT("PHYSICS_DEBUG"));			
			}
			FTimerHandle Handler;
			GetWorld()->GetTimerManager().SetTimer(Handler, this,&USHealthComp::CleanUp, 3.0f, false);
		}
		
	}
	onHealthChanged.Broadcast(this, this->health, Damage, InstigatedBy,  DamageCauser, ShotFromDirection, BoneName);
	//UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(health));

	//Using Broadcast to trigger blueprint
}

//TODO: Fix Damage/Health here.
//As it stands its putting off too much damage at a distance
void USHealthComp::HandleTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser ) {
	if (this->Role == ROLE_Authority) {
		if (Damage <= 0.0f)
		{		
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
				APlayerController* cont = Cast<APlayerController>(character->GetController());
				if (cont) {
					character->DisableInput(cont);
				}
				USkeletalMeshComponent* mesh = character->GetMesh();
				if (mesh) {
					mesh->SetSimulatePhysics(true);
					if (DamageCauser->IsA(ABarrelExplosionActor::StaticClass())) {
						ABarrelExplosionActor* barrel_exp_act = Cast<ABarrelExplosionActor>(DamageCauser);
						if (barrel_exp_act) {
							UE_LOG(LogTemp, Warning, TEXT("BarrelForce: %f"), barrel_exp_act->ExplosionForce);
							mesh->AddRadialForce(Origin, barrel_exp_act->SphereComp->GetScaledSphereRadius(),  barrel_exp_act->ExplosionForce, ERadialImpulseFalloff::RIF_Constant, true);
						}
					} else if (DamageCauser->IsA(AExplosionActor::StaticClass())) {
						AExplosionActor* gren_exp_act = Cast<AExplosionActor>(DamageCauser);
						if (gren_exp_act) {
							UE_LOG(LogTemp, Warning, TEXT("GrenForce: %f"), gren_exp_act->ExplosionForce);
							mesh->AddRadialForce(Origin, gren_exp_act->SphereComp->GetScaledSphereRadius(), gren_exp_act->ExplosionForce, ERadialImpulseFalloff::RIF_Constant, true);
						}
					}
				}
				USkeletalMeshComponent* weapon_mesh = character->GetWeapon()->GetWeaponMesh();
				if (weapon_mesh) {
					weapon_mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
					weapon_mesh->SetSimulatePhysics(true);
			
				}
				FTimerHandle Handler;
				GetWorld()->GetTimerManager().SetTimer(Handler, this,&USHealthComp::CleanUp, 3.0f, false);
			}
			
		}

	}
	FName defaultBone = "defaultBone";
	onHealthChanged.Broadcast(this, this->health, Damage, InstigatedBy,  DamageCauser, Origin, defaultBone);
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
}

float USHealthComp::GetHealth() {
	return this->health;
}
