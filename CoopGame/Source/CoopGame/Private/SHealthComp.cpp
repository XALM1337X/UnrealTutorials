// Fill out your copyright notice in the Description page of Project Settings.
#include "SHealthComp.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SCharacter.h"
#include "Components/PrimitiveComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "SWeapon.h"


// Sets default values for this component's properties
USHealthComp::USHealthComp()
{
	//PrimaryComponentTick.bCanEverTick = true;
	defaultHealth = 100;
	this->health = 0;
}


// Called when the game starts
void USHealthComp::BeginPlay()
{
	Super::BeginPlay();
	AActor* myOwner = GetOwner();
	this->health = this->defaultHealth;
	myOwner->OnTakePointDamage.AddDynamic(this, &USHealthComp::HandleTakePointDamage);	
}

void USHealthComp::HandleTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser ) 
{
	if (Damage <= 0.0f)
	{		
		return;
	}
	this->health = FMath::Clamp(this->health - Damage, 0.0f, this->defaultHealth);

	if (this->health <= 0.0f) {
		UPawnMovementComponent* MC = Cast<ASCharacter>(DamagedActor)->GetMovementComponent();
		if (MC) {
			MC->StopMovementImmediately();
		} 
		ASCharacter* character = Cast<ASCharacter>(DamagedActor);
		if (character) {
			USkeletalMeshComponent* mesh = character->GetMesh();
			if (mesh) {
				mesh->SetSimulatePhysics(true);
				mesh->AddImpulseAtLocation(ShotFromDirection*8000, ShotFromDirection ,BoneName);   //TODO: Remove magic number later with adjustable UPROPERTY.
			}
			USkeletalMeshComponent* weapon_mesh = character->GetWeapon()->GetWeaponMesh();
			if (weapon_mesh) {
				weapon_mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
				weapon_mesh->SetSimulatePhysics(true);
				weapon_mesh->WakeRigidBody();
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
