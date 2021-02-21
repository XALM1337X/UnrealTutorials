// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthCompMisc.h"


USHealthCompMisc::USHealthCompMisc() {
    
}

void USHealthCompMisc::BeginPlay()
{
	Super::BeginPlay();
	AActor* myOwner = GetOwner();
	this->defaultHealth = 40;
	this->health = this->defaultHealth;
	myOwner->OnTakePointDamage.AddDynamic(this, &USHealthCompMisc::HandleTakePointDamage);	
}

void USHealthCompMisc::HandleTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser) {
	if (Damage <= 0.0f)
	{		
		return;
	}
	this->health = FMath::Clamp(this->health - Damage, 0.0f, this->defaultHealth);

	if (this->health <= 0.0f) {
		//TODO: Spawn Explosion actor barrel here.


	}
}

