// Fill out your copyright notice in the Description page of Project Settings.


#include "BarrelHE.h"
#include "Components/StaticMeshComponent.h"
#include "SHealthCompMisc.h"

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

void ABarrelHE::ServerExplode_Implementation() {
	if (GetLocalRole() == ROLE_Authority && !isDead) {
		if (!this->isDead) {
			UE_LOG(LogTemp, Warning, TEXT("BOOM"));
			GetWorld()->GetTimerManager().SetTimer(Handler, this,&ABarrelHE::CleanUp, 5.0f, false);
			AActor* owner = GetOwner();
			this->isDead = true;
		}
		
		//Mesh thing will need to be done on net multicasted 
		//mesh->SetMaterial(0, OffMaterial);			
	}
}


void ABarrelHE::CleanUp_Implementation() {
	GetWorldTimerManager().ClearTimer(Handler);
	Destroy();
}

