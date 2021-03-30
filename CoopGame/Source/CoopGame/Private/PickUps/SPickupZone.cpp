// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/SPickupZone.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
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
	
}

void ASPickupZone::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);

}


