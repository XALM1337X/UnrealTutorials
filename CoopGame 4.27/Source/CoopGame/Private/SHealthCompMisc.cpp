// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthCompMisc.h"
#include "BarrelHE.h"


USHealthCompMisc::USHealthCompMisc() {
	if (GetOwnerRole() == ROLE_Authority) {
		this->defaultHealthBarrelHE = 60.0f;
		this->healthBarrelHE = 0;
	}
}

void USHealthCompMisc::BeginPlay() {
	Super::BeginPlay();
	if (GetOwnerRole() == ROLE_Authority) {
		this->healthBarrelHE = this->defaultHealthBarrelHE;
		AActor* myOwner = GetOwner();
		myOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthCompMisc::HandleTakeAnyDamageBarrelHE);	
	}
}

void USHealthCompMisc::HandleTakeAnyDamageBarrelHE(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {
	if (Damage <= 0.0f)	{		
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Current Health_1: %f"), healthBarrelHE);
	//UE_LOG(LogTemp, Warning, TEXT("Damage Delt: %f"), Damage);
	this->healthBarrelHE = FMath::Clamp(this->healthBarrelHE - Damage, 0.0f, this->defaultHealthBarrelHE);
	if (this->healthBarrelHE <= 0) {
		AActor* myOwner = GetOwner();
		ABarrelHE* barrel = Cast<ABarrelHE>(myOwner);
		if (barrel) {
			barrel->ServerExplode();
		}

	}
}

float USHealthCompMisc::GetHealth() {
	return this->healthBarrelHE;
}

