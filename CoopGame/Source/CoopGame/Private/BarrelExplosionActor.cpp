// Fill out your copyright notice in the Description page of Project Settings.


#include "BarrelExplosionActor.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"

ABarrelExplosionActor::ABarrelExplosionActor() {
    if (GetLocalRole() == ROLE_Authority) {
		PrimaryActorTick.bCanEverTick = true;
		SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("BarrelSphereComp"));
        RootComponent = SphereComp;
        ExplosionAnimationScale = FVector(6.0f, 6.0f, 6.0f);
		Ticks = 0;
		ExplosionForce = 25000;
	}
}

void ABarrelExplosionActor::BeginPlay() {
    if (GetLocalRole() == ROLE_Authority)
	{
		Super::BeginPlay();
		Explode();
	}
	UGameplayStatics::SpawnEmitterAtLocation(this, Explosion, GetActorLocation(),GetActorRotation(), ExplosionAnimationScale);
}

void ABarrelExplosionActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (Ticks >= 5)
	{
		Destroy();
	} 
	else	
	{
		Explode();
		Ticks++;
	}
}

void ABarrelExplosionActor::Explode_Implementation() {
	//UE_LOG(LogTemp, Warning, TEXT("HIT"));
	if (GetLocalRole() == ROLE_Authority)
	{
		TArray<UPrimitiveComponent*> OverLaps;
		SphereComp->GetOverlappingComponents(OverLaps);
		for (int32 i = 0; i < OverLaps.Num(); i++)
		{
			UPrimitiveComponent* OverLap = OverLaps[i];
			if (OverLap && OverLap->IsSimulatingPhysics())
			{
				//UE_LOG(LogTemp, Warning, TEXT("Overlapped component %s on actor %s"), *OverLap->GetName(),*OverLap->GetOwner()->GetName());
				OverLap->AddRadialForce(GetActorLocation(), SphereComp->GetScaledSphereRadius(), ExplosionForce, ERadialImpulseFalloff::RIF_Constant, true);
			}
		}
	}
}
