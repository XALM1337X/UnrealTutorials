// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/SPickupZone.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "PickUps/PickupPowerBase.h"
#include "GrenadeProjectile.h"
#include "Net/UnrealNetwork.h"
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
	if (GetLocalRole() == ROLE_Authority) {
		Respawn();
	}
}

void ASPickupZone::ServerRespawn() {
	this->Respawn();
}

void ASPickupZone::Respawn_Implementation() {
	if (GetLocalRole() != ROLE_Authority) {
		this->ServerRespawn();
	}
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
			PowerUpInstance->ActivatePickupPower(OtherActor);

			PowerUpInstance = nullptr;
			GetWorldTimerManager().SetTimer(TimerHandlerRespawn,this, &ASPickupZone::Respawn, CoolDownDuration); 
		}
	}
}


void ASPickupZone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPickupZone, PowerUpBase);
}