// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/PickupPowerBase.h"

// Sets default values
APickupPowerBase::APickupPowerBase() {
	PowerupInterval = 0.0f;
	TotalNumberOfTicks = 0;
	TicksProcessed = 0;

}

// Called when the game starts or when spawned
void APickupPowerBase::BeginPlay() {
	Super::BeginPlay();

}

void APickupPowerBase::TickPickupPower() {

	TicksProcessed++; 
	OnPickupPowerTicked();

	if (TicksProcessed >= TotalNumberOfTicks) {
		OnExpired(); 
		GetWorldTimerManager().ClearTimer(TimeHandle);
	}
}

void APickupPowerBase::ActivatePickupPower() {
	if (PowerupInterval > 0.0f) {
		GetWorldTimerManager().SetTimer(TimeHandle, this, &APickupPowerBase::TickPickupPower, PowerupInterval, true, 0.0f);
	} else {
		TickPickupPower();
	}
}

