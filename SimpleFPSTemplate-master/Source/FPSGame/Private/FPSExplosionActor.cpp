// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExplosionActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFPSExplosionActor::AFPSExplosionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SphereComp->SetupAttachment(MeshComp);
	Ticks = 0;

}

// Called when the game starts or when spawned
void AFPSExplosionActor::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::SpawnEmitterAtLocation(this, Explosion, GetActorLocation());	
	Explode();
}

// Called every frame
void AFPSExplosionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Ticks >= 5) {
		Destroy();
	}
	else {
		Explode();
		Ticks++;
	}
}

void AFPSExplosionActor::Explode()
{
	TArray<UPrimitiveComponent*> OverLaps;
	SphereComp->GetOverlappingComponents(OverLaps);
	for (int32 i = 0; i < OverLaps.Num(); i++)
	{
		UPrimitiveComponent* OverLap = OverLaps[i];
		if (OverLap && OverLap->IsSimulatingPhysics())
		{
			//UE_LOG(LogTemp, Warning, TEXT("Overlapped component %s on actor %s"), *OverLap->GetName(),*OverLap->GetOwner()->GetName());
			OverLap->AddRadialForce(GetActorLocation(), SphereComp->GetScaledSphereRadius(), 30000, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

