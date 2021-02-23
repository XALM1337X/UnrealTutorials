// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthCompMisc.h"
#include "BarrelHE.h"


USHealthCompMisc::USHealthCompMisc() {
	this->defaultHealthBarrelHE = 60.0f;
	this->healthBarrelHE = 0;
}

void USHealthCompMisc::BeginPlay() {
	Super::BeginPlay();
	this->healthBarrelHE = this->defaultHealthBarrelHE;
	AActor* myOwner = GetOwner();
	myOwner->OnTakePointDamage.AddDynamic(this, &USHealthCompMisc::HandleTakePointDamageBarrelHE);	
	myOwner->OnTakeRadialDamage.AddDynamic(this, &USHealthCompMisc::HandleTakeRadialDamageBarrelHE);
}
void USHealthCompMisc::HandleTakeRadialDamageBarrelHE(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser ) {
	
}
void USHealthCompMisc::HandleTakePointDamageBarrelHE(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser) {
	if (Damage <= 0.0f)
	{		
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Current Health_1: %f"), healthBarrelHE);
	//UE_LOG(LogTemp, Warning, TEXT("Damage Delt: %f"), Damage);
	this->healthBarrelHE = FMath::Clamp(this->healthBarrelHE - Damage, 0.0f, this->defaultHealthBarrelHE);
	if (this->healthBarrelHE <= 0) {
		AActor* myOwner = GetOwner();
		ABarrelHE* barrel = Cast<ABarrelHE>(myOwner);
		if (barrel) {
			barrel->TriggerExplosion(InstigatedBy);
		}

	}
	//UE_LOG(LogTemp, Warning, TEXT("Current Health_2: %f"), healthBarrelHE);
	//UE_LOG(LogTemp, Warning, TEXT("TRIGGER POINT DAMAGE"));
}


float USHealthCompMisc::GetHealth() {
	return this->healthBarrelHE;
}

