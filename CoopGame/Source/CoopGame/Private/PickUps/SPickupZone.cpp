// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/SPickupZone.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "PickUps/PickupPowerBase.h"
#include "GrenadeProjectile.h"
// Sets default values
ASPickupZone::ASPickupZone() {
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("PickupZoneSphereComp"));
	SphereComp->SetSphereRadius(75.0f);
	RootComponent = SphereComp;
	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("PickupZoneDecalComp"));
	DecalComp->SetupAttachment(SphereComp);
	DecalComp->SetRelativeRotation(FRotator(90.0f,0.0f,0.0f));
	DecalComp->DecalSize = FVector(64, 75, 75);

}

// Called when the game starts or when spawned
void ASPickupZone::BeginPlay() {
	Super::BeginPlay();
	Respawn();
}


void ASPickupZone::Respawn() {
	if (!PowerUpBase) {
		UE_LOG(LogTemp, Warning, TEXT("Respawn failure."));
		return;
	}
	FActorSpawnParameters actorSpawnParams;
	actorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PowerUpInstance = GetWorld()->SpawnActor<APickupPowerBase>(PowerUpBase, GetTransform(), actorSpawnParams);

}


void ASPickupZone::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
	if (!OtherActor->IsA(AGrenadeProjectile::StaticClass())) {
		if (PowerUpInstance) {
			PowerUpInstance->ActivatePickupPower();

			PowerUpInstance = nullptr;
			GetWorldTimerManager().SetTimer(TimerHandlerRespawn,this, &ASPickupZone::Respawn, CoolDownDuration); 
		}
	}
}
