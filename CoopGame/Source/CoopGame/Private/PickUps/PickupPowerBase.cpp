// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/PickupPowerBase.h"
#include "GameFramework/Actor.h"

// Sets default values
APickupPowerBase::APickupPowerBase() {
	PowerupInterval = 0.0f;
	TotalNumberOfTicks = 0;
	TicksProcessed = 0;

}

// Called when the game starts or when spawned
void APickupPowerBase::BeginPlay() {
	Super::BeginPlay();
	InitPower();
}

void APickupPowerBase::InitPower() {

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
		this->SetActorHiddenInGame(true);
	} else {
		OnActivated();
		this->SetActorHiddenInGame(true);
		GetWorldTimerManager().SetTimer(TimeHandle, this, &APickupPowerBase::OnExpired, 3.0f, true);
	}
}

