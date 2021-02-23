// Fill out your copyright notice in the Description page of Project Settings.


#include "BarrelHE.h"
#include "Components/StaticMeshComponent.h"
#include "SHealthCompMisc.h"
#include "BarrelExplosionActor.h"

// Sets default values
ABarrelHE::ABarrelHE() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelHEMesh"));
	RootComponent = mesh;
	HealthComp = CreateDefaultSubobject<USHealthCompMisc>(TEXT("HealthCompMiscBarrelHE"));
	isDead = false;
}

// Called when the game starts or when spawned
void ABarrelHE::BeginPlay() {
	Super::BeginPlay();
	
}

void ABarrelHE::TriggerExplosion(AController* controller) {
	if (GetLocalRole() == ROLE_Authority && !isDead) {
		//Call Explosion
		FVector loc = GetActorLocation();
		FRotator rot = GetActorRotation();
		AActor* spawn = GetWorld()->SpawnActor(ActorToSpawn, &loc, &rot);
		isDead = true;
		ABarrelExplosionActor* barrel_explosion = Cast<ABarrelExplosionActor>(spawn);
		if (barrel_explosion) {
			barrel_explosion->Init(controller);
		}
		FTimerHandle Handler;
		GetWorld()->GetTimerManager().SetTimer(Handler, this,&ABarrelHE::CleanUp, 5.0f, false);
		AActor* owner = GetOwner();
		mesh->SetMaterial(0, OffMaterial);			
	}
}


void ABarrelHE::CleanUp() {
	Destroy();
}

