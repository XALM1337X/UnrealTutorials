// Fill out your copyright notice in the Description page of Project Settings.


#include "BarrelHE.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SHealthCompMisc.h"

// Sets default values
ABarrelHE::ABarrelHE() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelHEMesh"));
	RootComponent = mesh;
	HealthComp = CreateDefaultSubobject<USHealthCompMisc>(TEXT("HealthCompMiscBarrelHE"));
	ExplosionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollisionSphere"));
	ExplosionSphere->SetupAttachment(mesh);
	this->ExplosionDamage = 150;
	this->ExplosionForce = 1000.0f;
	this->ExplosionAnimationScale = FVector(5.0f,5.0f,5.0f);
	isDead = false;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ABarrelHE::BeginPlay() {
	Super::BeginPlay();
	
}

void ABarrelHE::ServerExplode_Implementation() {
	bool Exploded = false;
	if (GetLocalRole() == ROLE_Authority && !isDead) {
		if (!this->isDead) {
			UE_LOG(LogTemp, Warning, TEXT("BOOM"));
			GetWorld()->GetTimerManager().SetTimer(Handler, this,&ABarrelHE::CleanUp, 5.0f, false);
			AActor* owner = GetOwner();
			this->isDead = true;
			TArray<AActor*> ignores;
			UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), ExplosionSphere->GetScaledSphereRadius(), damageType, ignores, this, GetInstigatorController(), false, ECC_WorldDynamic);
			Exploded = true;
		}			
	} 

	if (Exploded) {
		PlayExplosionEffect();
	}	
}

void ABarrelHE::PlayExplosionEffect_Implementation() {
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, GetActorLocation(), GetActorRotation(), this->ExplosionAnimationScale);
	mesh->SetMaterial(0, OffMaterial);
}
void ABarrelHE::CleanUp_Implementation() {
	GetWorldTimerManager().ClearTimer(Handler);
	Destroy();
}

