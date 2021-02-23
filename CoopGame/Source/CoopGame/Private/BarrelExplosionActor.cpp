// Fill out your copyright notice in the Description page of Project Settings.


#include "BarrelExplosionActor.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"

ABarrelExplosionActor::ABarrelExplosionActor() {
    if (GetLocalRole() == ROLE_Authority) {
		PrimaryActorTick.bCanEverTick = true;
		this->SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("BarrelSphereComp"));
        RootComponent = this->SphereComp;
        this->ExplosionAnimationScale = FVector(6.0f, 6.0f, 6.0f);
		this->Ticks = 0;
		this->ExplosionForce = 140000.0f;
	}
}

void ABarrelExplosionActor::Init(AController* controller) {
	this->instigator = controller;
}

void ABarrelExplosionActor::BeginPlay() {
    if (GetLocalRole() == ROLE_Authority)
	{
		Super::BeginPlay();
	}
}

void ABarrelExplosionActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	UGameplayStatics::SpawnEmitterAtLocation(this, this->Explosion, GetActorLocation(),GetActorRotation(), this->ExplosionAnimationScale);
	if (GetLocalRole() == ROLE_Authority) {
		this->Explode();
		Destroy();
	}
}

void ABarrelExplosionActor::Explode_Implementation() {
	if (GetLocalRole() == ROLE_Authority)
	{
		TArray<AActor*> ignores;
		UGameplayStatics::ApplyRadialDamage(GetWorld(), 120, GetActorLocation(), this->SphereComp->GetScaledSphereRadius(), this->damageTypeHEBarrel, ignores, this, this->instigator, false, ECC_WorldDynamic);		
	}
}
