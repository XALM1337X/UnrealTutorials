// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionActor.h"
#include "Components/SphereComponent.h"
#include "kismet/GameplayStatics.h"
// Sets default values
AExplosionActor::AExplosionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	if (GetLocalRole() == ROLE_Authority)
	{
		PrimaryActorTick.bCanEverTick = true;
		SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
		RootComponent = SphereComp;
		Ticks = 0;
	}
}

// Called when the game starts or when spawned
void AExplosionActor::BeginPlay()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Super::BeginPlay();
		Explode();
	}
	UGameplayStatics::SpawnEmitterAtLocation(this, Explosion, GetActorLocation(),GetActorRotation(),FVector(3.0f,3.0f,3.0f));
	
}

// Called every frame
void AExplosionActor::Tick(float DeltaTime)
{
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

void AExplosionActor::Explode_Implementation()
{
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
				OverLap->AddRadialForce(GetActorLocation(), SphereComp->GetScaledSphereRadius(), 20000, ERadialImpulseFalloff::RIF_Constant, true);
			}
		}
	}
}

bool AExplosionActor::Explode_Validate()
{
	return true;
}

